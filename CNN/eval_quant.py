"""
量化精度评估脚本
原理：把 float 输入做一次 int8 量化→反量化，对比 float 模型在
      原始输入 vs 量化震荡输入上的输出差异，衡量 int8 量化损失。
      
三步评估：
  1. Float 精度 — Keras 模型在原始 test set 上的准确率
  2. 输入量化损失 — 输入 ×32→round→÷32 的 MSE 和被截断比例
  3. 量化反推精度 — 用量化后的输入喂模型，看准确率掉多少
"""

import numpy as np
import re, os
from tensorflow.keras.models import load_model
from tensorflow.keras import utils

# ===== 被量化的参数，需和 weights.h 保持一致 =====
INPUT_DEC = 5               # INPUT_1_OUTPUT_DEC
SCALE = 2 ** INPUT_DEC       # 32
OUPUT_THRESHOLD = 103        # int8 置信度阈值

# ===== 复刻训练脚本的数据加载逻辑 =====
DEF_SAVE_TO_PATH = './TraningData_6_21/'
DEF_FILE_FORMAT = '.txt'
DEF_N_ROWS = 150
DEF_USE_COLS = (0, 1, 2)
DEF_FILE_MAX = 100
motion_names = ['RightAngle', 'SharpAngle', 'Lightning', 'Triangle',
                'Letter_h', 'letter_R', 'letter_W', 'letter_phi',
                'Circle', 'UpAndDown', 'Horn', 'Wave', 'NoMotion']
motion_to_label = {name: idx for idx, name in enumerate(motion_names)}

print("=" * 60)
print("1. 加载训练时保存的 model.h5 和测试数据")
print("=" * 60)

model = load_model('model.h5')
model.compile(loss='categorical_crossentropy', metrics=['accuracy'])

def load_dataset(root_dir, max_rows=DEF_N_ROWS):
    file_list, labels = [], []
    for fn in os.listdir(root_dir):
        if fn.endswith(DEF_FILE_FORMAT):
            m = re.match(rf'^([\w]+)_([\d]+){DEF_FILE_FORMAT}$', fn)
            if m and 0 <= int(m.group(2)) <= DEF_FILE_MAX:
                if m.group(1) in motion_to_label:
                    data = np.loadtxt(os.path.join(root_dir, fn),
                                      delimiter=' ', usecols=DEF_USE_COLS,
                                      max_rows=max_rows)
                    file_list.append(data)
                    labels.append(motion_to_label[m.group(1)])
    return file_list, labels

file_list, labels = load_dataset(DEF_SAVE_TO_PATH)
# pad_sequences
max_len = max(len(x) for x in file_list)
from tensorflow.keras.preprocessing.sequence import pad_sequences
data = pad_sequences(file_list, maxlen=max_len, dtype='float32',
                     padding='post', value=0)
labels_oh = utils.to_categorical(labels, num_classes=len(motion_names))

# 用同样的 shuffle + 80/20 分割（必须和训练时一致才能对比）
num = len(data)
indices = np.arange(num)
np.random.seed(0)  # 固定种子保证可复现
np.random.shuffle(indices)
train_sz = int(num * 0.8)
x_test = data[indices[train_sz:]]
y_test = labels_oh[indices[train_sz:]]
y_test_int = np.array(labels)[indices[train_sz:]]  # 非 one-hot，用于算正确数

print(f"  测试集 {x_test.shape[0]} 条, 输入形状 {x_test.shape}")

# ===== 2. Float 精度 =====
print("\n" + "=" * 60)
print("2. Float 模型在测试集上的精度")
print("=" * 60)

scores = model.evaluate(x_test, y_test, verbose=2)
float_loss, float_acc = scores[0], scores[1]
print(f"  Float loss:     {float_loss:.4f}")
print(f"  Float accuracy: {float_acc:.2%}")

# ===== 3. 输入量化损失 =====
print("\n" + "=" * 60)
print("3. 输入量化损失分析")
print("=" * 60)

x_flat = x_test.flatten()
x_scaled = x_flat * SCALE              # Step 1: ×32
x_quant = np.clip(np.round(x_scaled), -128, 127).astype(np.int8)  # Step 2: round + clamp
x_recon = x_quant.astype(np.float32) / SCALE  # Step 3: 反量化

# MSE
mse = np.mean((x_flat - x_recon) ** 2)
# 截断统计
clipped_low = np.sum(x_scaled < -128)
clipped_high = np.sum(x_scaled > 127)
clipped = clipped_low + clipped_high

print(f"  输入值范围:  [{x_flat.min():+.3f}, {x_flat.max():+.3f}] g")
print(f"  ×32 后范围: [{x_scaled.min():+.1f}, {x_scaled.max():+.1f}]")
print(f"  截断 (<-128): {clipped_low}   (>127): {clipped_high}  占比: {clipped}/{x_flat.size} ({100*clipped/x_flat.size:.2f}%)")
print(f"  量化 MSE:    {mse:.6f}  (越小越好，<0.001 可视为无损失)")
print(f"  分辨率:      1/32 = {1/SCALE:.4f} g/LSB")

# 分轴统计
for axis, name in enumerate(['AccX', 'AccY', 'AccZ']):
    axis_data = x_test[:, :, axis].flatten()
    axis_scaled = axis_data * SCALE
    axis_clip = np.sum(np.abs(axis_scaled) > 127)
    print(f"  {name}: [{axis_data.min():+.2f}, {axis_data.max():+.2f}] g, "
          f"截断 {axis_clip}/{len(axis_data)} ({100*axis_clip/len(axis_data):.2f}%)")

# ===== 4. 量化反推精度（关键指标） =====
print("\n" + "=" * 60)
print("4. 量化反推精度 (核心指标)")
print("=" * 60)
print("  方法: 将输入量化再反量化后喂给 float 模型，")
print("  模拟 MCU 上 int8 首层输入的最坏情况精度损失。")
print("  注意: 这只能测输入量化的影响，中间层累加误差测不到。")

# 对每条测试数据做量化→反量化，然后预测
y_pred_float = model.predict(x_test, verbose=0)
y_pred_float_label = np.argmax(y_pred_float, axis=1)

# 量化输入
x_test_q = np.clip(np.round(x_test * SCALE), -128, 127).astype(np.float32) / SCALE
y_pred_quant = model.predict(x_test_q, verbose=0)
y_pred_quant_label = np.argmax(y_pred_quant, axis=1)

float_correct = np.sum(y_pred_float_label == y_test_int)
quant_correct = np.sum(y_pred_quant_label == y_test_int)
total = len(y_test_int)

print(f"\n  Float 正确:     {float_correct}/{total}  = {100*float_correct/total:.1f}%")
print(f"  量化反推正确:   {quant_correct}/{total}  = {100*quant_correct/total:.1f}%")
print(f"  输入量化损失:   {float_correct - quant_correct} 条 ({100*(float_correct-quant_correct)/total:.1f}%)")

# 逐条对比差异
print(f"\n  逐条 Softmax 最大概率值差异 (反推 - 原始):")
diff = np.max(y_pred_quant, axis=1) - np.max(y_pred_float, axis=1)
print(f"    均值: {np.mean(diff):+.4f}  标准差: {np.std(diff):.4f}  最大: {np.max(diff):+.4f}  最小: {np.min(diff):+.4f}")

# 哪些被量化改变了结果
changed = np.where(y_pred_float_label != y_pred_quant_label)[0]
if len(changed) > 0:
    print(f"\n  被输入量化改变了分类结果的样本: {len(changed)} 条")
    for c in changed[:5]:
        print(f"    样本#{c}: {motion_names[y_pred_float_label[c]]} → {motion_names[y_pred_quant_label[c]]}")
else:
    print(f"\n  输入量化没有改变任何分类结果 ✓")

# ===== 5. 总结 =====
print("\n" + "=" * 60)
print("5. 评估总结")
print("=" * 60)
print(f"""
  模型架构:      Conv1D(30)→ReLU→Conv1D(15)→ReLU→MaxPool→Dense(13)→Softmax
  参数量:        2653 (10.36 KB)
  输入量化:      float g × 32 → round → clip[-128,127] → int8
  量化分辨率:     1/32 = 0.03125 g/LSB

  Float 精度:     {float_acc:.1%}
  输入量化反推:   {100*quant_correct/total:.1%}  (-{100*(float_correct-quant_correct)/total:.1f}%)
  输入截断率:     {100*clipped/x_flat.size:.2f}%
  量化 MSE:       {mse:.6f}

  ⚠ 反推精度只反映首层输入量化的损失。
     中间层 Conv/Dense 的累积量化误差需在 MCU 实测。
     通常 int8 量化总损失 = 输入损失 + 层间损失 ≈ 2~5%。
""")

if clipped == 0 and quant_correct == float_correct:
    print("  ✓ 输入量化零损失，精度与 float 完全一致")
elif quant_correct >= float_correct * 0.98:
    print("  ✓ 输入量化损失 < 2%，在可接受范围内")
else:
    print("  ⚠ 输入量化损失 > 2%，建议检查数据范围或调整 INPUT_DEC")

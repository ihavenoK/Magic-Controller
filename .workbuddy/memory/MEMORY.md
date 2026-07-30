# CyberryPotter 项目笔记

## 已修复 (2026-06-14)

### ✅ B1+B2 + 统一改用加速度
- `User/config.h:60`: `IMU_GYRO_TRANS_RADIAN_CONSTANT` 8192.0 → 3755.0
- `main.c:39-41`: `model_feed_data()` 从 `IMU_Data_mGyro` 改为 `IMU_Data_mAcc`，注释同步修正
- `CNN/CNNTrainRaw.py:22`: `DEF_USE_COLS` 从 `(3,4,5)` 改为 `(0,1,2)`
- **重要**: 需重新采集训练数据并用 CNNTrainRaw.py 重新训练生成 weights.h，否则旧模型推理结果随机

## 训练结果 (2026-06-21)

### 数据集
- TraningData_6_21/: 每条动作约 12 组，13 类共约 185 条
- 输入: 加速度 3 轴 (AccX,AccY,AccZ)，DEF_USE_COLS=(0,1,2)
- 150 帧 × 3 通道，80/20 分割

### 训练
- 架构: Conv1D(30,3)→ReLU→Conv1D(15,3)→ReLU→MaxPool1D(3)→Dense(13)→Softmax
- 参数量: 2653 (10.36 KB)
- val_accuracy 首次 96% @ Epoch 96, val_loss 最低 0.42 @ Epoch 121
- EarlyStopping 在 Epoch 131 触发 (val_loss 连续 10 轮未改善)
- ModelCheckpoint 按 val_accuracy 保存 → 保存了 Epoch 96 的模型（非最优）

### 量化评估 (eval_quant.py)
- Float 精度: 100% (25 条测试)
- 输入量化反推: 100%，零损失
- 加速度范围 [-1.09g, +2.33g], ×32 后 [−34.7, +74.4], 远离 ±128 截断
- MSE: 0.000081, softmax 置信度差异均值 −0.0013
- 中间层累积误差需 MCU 实测

### 待改进
- 数据量偏少（每类 ~12 条），建议补到 ≥30
- ModelCheckpoint 应改为 monitor='val_loss' 获取最优泛化模型
- 当前 weights.h 对应 Epoch 96 (val_loss=0.57)，Epoch 117~121 (val_loss=0.42) 更优

## 项目规则 (2026-07-29)

- `.workbuddy/rules/embedded-c-standards.md`：提取自《嵌入式C项目开发规范手册》v3.0，含 13 章 + 附录
- 已标注每节对 CyberryPotter 的适用性（🔴强制/🟡建议/⚪不适用）
- 关键强制项：stdint.h 类型、静态分配、增量构建、喂狗策略、CRC 校验

## 红外模块接入 (2026-07-29)

### 硬件
- v1221 (IR TX): PA15 (TIM2_CH1) → 38kHz PWM 载波
- v1222 (IR RX): PB0 (TIM3_CH3) → 输入捕获 NEC 解码
- Flash 存储: 0x0801F800 (F103CBT6 最后一页 2KB)

### 代码
- 新增 `User/IR.h` + `User/IR.c`：NEC 发射/接收/Flash 存读
- 系统模式简化为两态：SYSTEM_GESTURE_IR ⇄ SYSTEM_IR_LEARN（超长按切换）
- CNN 识别后自动调用 IR_Send_NEC() 发射
- 学习模式：短按切位槽（PA3~PA6 二进制显示），外部遥控对准 V1222 自动学码→存 Flash

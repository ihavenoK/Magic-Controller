# Magic Controller

基于 STM32F103 的 TinyML 实时手势识别魔杖。使用 MPU6050 采集 6 轴惯性数据，通过 NNoM 框架在 MCU 端部署 int8 量化 CNN 模型，识别 13 种挥动动作（法术），并支持红外 RAW 信号的学习与发射——无需专用 PCB，仅用面包板与通用模块即可搭建。

## 核心特性

- **CNN 实时推理** — 2 层 Conv1D + Dense 架构，2653 个参数（10.36 KB），int8 量化后在 STM32F103 上直接运行
- **13 种法术识别** — RightAngle、SharpAngle、Lightning、Triangle、Letter_h、letter_R、letter_W、letter_phi、Circle、UpAndDown、Horn、Wave、NoMotion
- **红外 RAW 万能学习** — 不依赖特定协议，直接录制/回放 38kHz 载波脉冲序列，兼容市面绝大多数红外遥控器
- **完整工具链** — 配套 Python 数据采集脚本、训练脚本、量化评估脚本、Web 串口助手
- **低硬件门槛** — STM32F103C8T6/CBT6 最小系统板 + MPU6050 + 按键 + 5 个 LED 即可运行

## 硬件需求

| 模块 | 型号/参数 | 数量 |
|------|-----------|------|
| MCU | STM32F103C8T6 / CBT6 最小系统板 | 1 |
| IMU | MPU6050 (GY-521) | 1 |
| USB 转串口 | CH340 模块 | 1 |
| LED | 5 mm，颜色任选 | 5 |
| 限流电阻 | 1 kΩ ~ 10 kΩ | 5 |
| 按键 | 四脚轻触开关 | 1 |
| 红外发射管 (可选) | 38 kHz 红外 LED | 1 |
| 红外接收头 (可选) | VS1838B 或同类 | 1 |

### 引脚连接

```
MPU6050:  VCC → 3.3 V    GND → GND    SCL → PB6    SDA → PB7
LED1-4:   PA3, PA4, PA5, PA6（各串限流电阻到 GND）
主 LED:    PA7（串限流电阻到 GND）
按键:      PA0 → GND（使用内部上拉，按下为低电平）
串口:      PA9 (TX), PA10 (RX) → CH340
红外发射:   PA15 → 红外 LED（可选）
红外接收:   PB0  → VS1838B OUT（可选）
```

## 环境配置

### MCU 固件开发 (Keil MDK)

- **IDE:** Keil MDK v5（请使用官网最新版）
- **编译器:** Arm Compiler 6.22
- **依赖包（需精确版本）:**

| 包名 | 版本 | 下载 |
|------|------|------|
| CMSIS | 6.0.0 | [keil.arm.com](https://www.keil.arm.com/packs/cmsis-arm/versions/) |
| CMSIS Compiler | 2.1.0 | [keil.arm.com](https://www.keil.arm.com/packs/cmsis-compiler-arm/versions/) |
| STM32F1xx_DFP | 2.4.1 | [keil.arm.com](https://www.keil.arm.com/packs/stm32f1xx_dfp-keil/versions/) |

打开 `CyberryPotter.uvprojx`，Keil 会自动提示安装缺失的包，按提示操作即可。

### Python 环境 (数据采集 / 模型训练)

环境要求：Python 3.9，conda 或 venv 均可。

```bash
# 1. 创建并激活环境
conda create --name cyberry python=3.9
conda activate cyberry

# 2. 安装依赖（使用清华镜像加速）
cd CNN
pip install --index-url https://pypi.tuna.tsinghua.edu.cn/simple -r requirements.txt
```

核心依赖：`tensorflow-cpu==2.14.1`、`keras==2.14.0`、`numpy==1.26.4`、`scikit-learn==1.5.1`、`pyserial==3.5`、NNoM（`CNN/nnom-master/`，已内置于项目目录）。

## 项目结构

```
cyberrypotter/
├── main.c                          # 主循环：模式切换、CNN 推理、LED 控制、IR 管理
├── User/
│   ├── config.h                    # 全局配置（系统模式、引脚、IMU 参数、CNN 阈值）
│   ├── CyberryPotter.h / .c        # 系统状态机、初始化、按键/IMU 中断
│   ├── IMU.h / .c                  # MPU6050 数据采集与串口打印
│   ├── IR.h / .c                   # 红外 RAW 收发、Flash 存储
│   └── Delay.h / .c                # SysTick 精确延时
├── STM32_Hardware/
│   ├── hardware.h / .c             # 硬件初始化（RCC、GPIO、USART、TIM、NVIC）
│   ├── IIC.h / .c                  # 软件 I2C（PB6/PB7，约 200 kHz）
│   ├── MPU6050.h / .c / MPU6050_Reg.h  # DMP 初始化与寄存器定义
│   └── eMPL/                       # InvenSense 官方 eMPL DMP 驱动
├── CNN/
│   ├── Serial_Read.py              # 数据采集脚本
│   ├── CNNTrainRaw.py              # 模型训练 + NNoM 权重导出
│   ├── CNNTestSerialRaw.py         # 串口实时推理测试
│   ├── eval_quant.py               # int8 量化精度评估
│   ├── weights.h                   # NNoM 模型图 + 量化权重
│   ├── requirements.txt            # Python 依赖清单
│   ├── TraningData_6_21/           # 训练数据集（约 185 条，13 类）
│   ├── nnom-master/                # NNoM 嵌入式推理框架（v0.4.x）
│   └── SpellsCard/                 # 13 种法术打印卡片
├── RTE/                            # CMSIS 启动文件 + 设备配置
├── Web串口助手.html                 # 纯前端 Web Serial API 串口工具
└── Schematic.jpg                   # 硬件原理图
```

## CNN 模型架构

```
Input(150, 3)          ← 150 时间步 × 3 轴加速度 (AccX, AccY, AccZ)，100 Hz 采样 1.5 s
   │
Conv1D(30, kernel=3, stride=3) → ReLU
   │
Conv1D(15, kernel=3, stride=3) → ReLU
   │
MaxPool1D(pool=3, stride=3)
   │
Flatten → Dense(13) → Dropout(0.5) → Softmax
```

| 参数 | 值 |
|------|-----|
| 总参数量 | 2,653 (10.36 KB) |
| 量化格式 | int8 per-tensor，位宽 8 bit |
| 输入量化 | float(g) × 32 → round → clip[-128, 127] |
| 输入分辨率 | 1/32 ≈ 0.03125 g/LSB |
| 置信度阈值 | 103/127 (≈81%)，低于此值判为 Unrecognized |
| val_accuracy | 96%（当前预训练权重，Epoch 96） |

## 使用示例

### 示例一：手势识别 + 红外遥控

**场景：** 将魔杖作为万能遥控器，不同挥动动作控制不同家电。

**步骤：**

1. **学习红外码** — 长按按键 2.5 s 进入学习模式（主 LED 熄灭）。短按切换槽位（0~12，PA3~PA6 以二进制显示当前槽位），对着红外接收头按下遥控器按键，收到信号后主 LED 快闪 4 次 → 常亮 1 s 表示学习成功。

2. **挥动识别 + 发射** — 长按按键 2.5 s 切回手势模式，短按按键后挥动魔杖，识别到法术即自动通过红外发射头重放对应槽位存储的遥控码。

**代码关键路径：**

```c
// main.c — 手势模式下的推理与 IR 发射
if (Cyberry_Potter_Status.IMU_Status == IMU_Sampled) {
    model_feed_data();       // 加速度数据 × 32 → int8 填入模型输入
    model_run(model);        // NNoM 推理
    model_output = model_get_output();  // 获取置信度最高的分类

    // 自动查找 Flash 中对应法术的红外码并发射
    if (model_output >= 0 && model_output < IR_GESTURE_COUNT) {
        IR_Raw_Signal_t raw_sig;
        if (IR_Flash_Load((uint8_t)model_output, &raw_sig)) {
            IR_Send_Raw(&raw_sig);   // 38 kHz PWM 逐脉冲回放
        }
    }
}
```

```python
# CNN/CNNTrainRaw.py — 模型训练核心
model.compile(
    optimizer=optimizers.Adam(),
    loss=losses.CategoricalCrossentropy(),
    metrics=['accuracy']
)

history = model.fit(
    x_train, y_train,
    batch_size=80, epochs=200,
    validation_data=(x_test, y_test),
    callbacks=[
        callbacks.EarlyStopping(monitor='val_loss', patience=10),
        callbacks.ModelCheckpoint('model.h5', monitor='val_accuracy',
                                  save_best_only=True, mode='max')
    ]
)

# 导出 NNoM int8 量化权重文件
generate_model(model, x_test[:100], format='hwc', name='weights.h')
```

### 示例二：从头训练自己的手势模型

**场景：** 自定义一套完全不同的手势集合，或在你的硬件上重新训练以获得更高精度。

**步骤：**

1. **修改配置进入采集模式** — 打开 `User/config.h`，取消注释 `#define SYSTEM_MODE_DATA_COLLECT`，编译烧录。

2. **采集数据** — 运行 `CNN/Serial_Read.py`，选择 COM 口，按键触发后挥动魔杖，脚本自动接收 150 帧数据并保存为 `TraningData_MM_DD/ClassName_XX.txt`。

3. **训练模型** — 将采集数据放入 `CNN/TraningData_6_21/`（或修改 `CNNTrainRaw.py` 第 16 行的 `DEF_SAVE_TO_PATH`），运行：

```bash
python CNNTrainRaw.py
```

训练完成后生成 `model.h5`（float 模型）和 `weights.h`（NNoM 量化权重）。

4. **部署到 MCU** — 将生成的 `weights.h` 替换 `CNN/weights.h`，注释掉 `SYSTEM_MODE_DATA_COLLECT`，重新编译烧录。

5. **串口验证（可选）** — 运行 `CNN/CNNTestSerialRaw.py`，在 PC 端用 float 模型实时推理串口数据，与 MCU 端 int8 推理结果对比。

**量化精度评估：**

```bash
python eval_quant.py
```

输出 float 精度、输入量化反推精度、MSE 及 softmax 置信度差异。当前模型：float 精度 100%（测试集 25 条），输入量化零损失，MSE = 0.000081。

## 系统工作模式

| 模式 | 进入方式 | 功能 |
|------|----------|------|
| `SYSTEM_GESTURE_IR` (默认) | 上电默认 | 短按 → IMU 采样 → CNN 推理 → LED + IR 发射 |
| `SYSTEM_IR_LEARN` | 超长按 2.5 s | 短按切槽位 / 长按清空 / 对准遥控器自动学码 ```


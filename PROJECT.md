# Cyberry Potter · 电子法棒

> 在 **STM32F103C8T6（Cortex-M3, 72MHz, 20KB SRAM）** 上部署卷积神经网络，实现 12 种手势轨迹的实时识别与红外遥控。

![gestures](assets/1.png)

## 特性

- **端侧推理** — 无需上位机/云端，NNOM 框架在 STM32 上跑 CNN 前向推理
- **12 种手势** — 覆盖直角、锐角、闪电、三角、字母(h/R/W/φ)、画圆、上下、牛角、波浪
- **实时响应** — MPU6050 采集 6 轴 IMU 数据 → CNN 分类 → 红外发射，全链路 < 100ms
- **零硬件加速器** — 纯 CPU 推理，模型大小 < 20KB，RAM 占用 < 10KB
- **QT 上位机** — 数据采集、标注、训练、模型导出一条龙
- **模块化设计** — MainBoard/Module 架构让同一套框架适配不同应用（红外遥控、手势鼠标等）

## 系统架构

```
┌─────────────────────────────────────────────────┐
│                    训练流程（PC 端）               │
│  MPU6050 原始数据 ──▶ 上位机采集标注 ──▶ Keras 训练 │
│                                │                │
│                    NNOM 工具链转换                 │
│                         ↓                        │
│                     weights.h                    │
│                     params.h                     │
└─────────────────────────────────────────────────┘
                         │ 烧录
┌────────────────────────▼────────────────────────┐
│                    推理流程（STM32 端）            │
│                                                  │
│  ┌──────────┐   I2C   ┌──────────┐   UART   ┌──┐│
│  │ MPU6050  │────────▶│ STM32F103│────────▶│红外││
│  │ 6轴传感器 │  Ax~Gz  │ NNOM CNN │  编码   │发射││
│  └──────────┘         └──────────┘         └──┘│
│                            │                    │
│                     200Hz DMP 中断触发            │
│                     150 点轨迹 → CNN 分类         │
│                     12+1 (NoMotion) 类别输出      │
└─────────────────────────────────────────────────┘
```

| 环节 | 技术 | 说明 |
|------|------|------|
| 数据采集 | MPU6050 @ 200Hz | 3轴加速度 + 3轴陀螺仪 |
| 上位机 | Python + PyQT5 | 串口采集、实时波形、标注、训练 |
| 模型训练 | Keras (TensorFlow 2.x) | 1D-CNN，输入 [150,3] 或 [150,6]，输出 13 类 |
| 模型转换 | NNOM 工具链 | Keras → C 权重数组（weights.h） |
| 推理框架 | NNOM v0.4.x | 纯 ANSI C，自动内存规划 |
| MCU | STM32F103C8T6 | 72MHz Cortex-M3, 64KB Flash, 20KB SRAM |

## 项目结构

```
电子法棒/
├── Software/                  # 主固件工程（Keil MDK）
│   ├── MainBoard/             # 核心业务层
│   │   ├── main.c             # 主函数 + CNN 推理封装
│   │   ├── CyberryPotter.c    # 系统初始化 + 模块调度
│   │   ├── CyberryPotter.h    # 枚举、结构体、外部声明
│   │   ├── config.h           # 编译开关（模式选择）
│   │   ├── IMU.c / IMU.h      # IMU 采样逻辑
│   │   ├── MPU6050.c/.h       # MPU6050 DMP 驱动
│   │   ├── IIC.c/.h           # 软件 I2C 实现
│   │   ├── USART.c/.h         # 串口驱动
│   │   ├── LED.c/.h           # LED 控制
│   │   ├── button.c/.h        # 按键状态机
│   │   ├── new_study_IR.c/.h  # 红外子板通信
│   │   ├── protocol.c/.h      # 红外协议数据包
│   │   └── eMPL/              # InvenSense DMP 姿态解算库
│   ├── Module/                # 可插拔功能模块（11 个）
│   │   ├── module0.c/.h       # 红外遥控发射/学习（主用）
│   │   └── module1~10.c/.h    # 预留给其他应用场景
│   ├── CNN/                   # NNOM 推理库源码
│   └── Project/               # Keil 工程文件（.uvprojx）
├── NNOM_Demo/                 # NNOM 官方示例（含 CNN 入门例程）
├── Upper_computer/            # QT 上位机（已编译的 .exe）
│   ├── Acceptance_of_data/    # 数据采集工具
│   └── Train_model/           # 模型训练工具
├── hardware/                  # 硬件设计
│   ├── cyberry potter.epro    # 立创 EDA 工程
│   ├── BOM.xlsx               # 物料清单
│   └── GERBER.zip             # 打板文件
├── 3D_print/                  # 外壳 3D 模型
├── Model_trajectories/        # 12 种手势轨迹示意图
├── assets/                    # 文档配图
└── Install_package/           # 开发工具包
    ├── MDK5.28.exe            # Keil MDK
    ├── XCOM V2.2.exe          # 串口助手
    └── weights.h              # 预训练权重
```

## 快速开始

### 1. 环境准备

- **IDE：** Keil MDK v5（需安装 STM32F1 器件包）
- **烧录工具：** ST-Link / J-Link / USB-TTL 串口下载
- **串口助手：** XCOM 或任意串口工具（115200-8-N-1）

### 2. 编译烧录

```
打开 Software/Project/cyberry potter.uvprojx
编译 → 下载到 STM32F103C8T6
将 Install_package/weights.h 放到 Software/MainBoard/ 目录覆盖原文件
```

### 3. 上电使用

1. 法棒水平静止放置 → 上电
2. `KEY0` = 开始采集，LED 灯亮 → 挥动手势 → `KEY0` 结束采集
3. LED 闪烁 N 次 = 识别结果（结果码见下表）
4. `KEY1` = 红外发射（控制对应的家电设备）

> 采集前保持静止的 1 秒窗口用于自动校准 MPU6050 零偏。

## 支持的手势

| 编号 | 手势 | 图示 | 编号 | 手势 | 图示 |
|:---:|------|------|:---:|------|------|
| ① | 直角 | `Right_Angle` | ⑦ | 字母 R | `Letter_R` |
| ② | 锐角 | `Sharp_Angle` | ⑧ | 字母 W | `Letter_W` |
| ③ | 闪电 | `Lightning` | ⑨ | 字母 Φ | `Letter_Phi` |
| ④ | 三角形 | `Triangle` | ⑩ | 画圆 | `Circle` |
| ⑤ | 字母 h | `Letter_H` | ⑪ | 上下 | `Up_And_Down` |
| ⑥ | 牛角 | `Horn_` | ⑫ | 波浪 | `Wave` |

> `NoMotion（静止）` 为默认负类，不计入手势列表。

## 识别流水线

```
MPU6050 原始数据（3轴加速度 + 3轴陀螺仪，200Hz）
        │
        ▼ DMP 姿态解算后截取 150 点
[150, 3] 轨迹序列矩阵
        │
        ▼ NNOM 1D-CNN 推理
13 维 Softmax 概率向量
        │
        ▼ Argmax + 置信度阈值
手势标签（0~12）
        │
        ▼ 红外协议编码
38KHz 红外载波发射 → 控制家电
```

## 硬件方案

| 模块 | 型号 | 接口 | 说明 |
|------|------|------|------|
| MCU | STM32F103C8T6 | — | Cortex-M3, 72MHz, 64K/20K |
| IMU | MPU6050 | I2C | 6 轴, ±16g / ±2000°/s |
| 红外发射 | IR LED + STM8 子板 | UART2 (PA2/PA3) | 38KHz 载波调制，主控通过串口协议控制 |
| 红外接收 | VS1838B（子板） | — | NEC 协议学习，红外学习子板独立处理 |
| 电池 | 250mAh 锂聚合物 | — | 3.7V |
| 充电 | TP4056 | USB Micro-B | 1S 锂电池充电管理 |
| 显示 | 0.91" OLED | I2C (可选) | SSD1306 |
| 按键 | 轻触开关 ×2 | GPIO | 采集触发 / 发射触发 |

PCB 设计工具为 **立创 EDA**，双面板，外形适配法棒手柄。

## 训练自定义手势

### Step 1 — 采集数据

法棒切换到数据采集模式（`config.h` 中启用 `SYSTEM_MODE_DATA_COLLECT`），
串口连接后启动上位机 `Upper_computer/Acceptance_of_data/Acceptance_of_data.exe`。

用串口连接法棒，在 QT 界面中逐手势采集样本，每个手势建议 ≥ 50 组。

### Step 2 — 训练模型

在 QT 上位机中点击 **Train**，背后流程：

```
Keras 1D-CNN 训练 → 导出 .h5 模型
      │
NNOM 工具链转换 → weights.h + params.h
```

### Step 3 — 替换部署

将生成的 `weights.h` 覆盖 `Software/MainBoard/weights.h`，重新编译烧录即生效。

## 二次开发

项目采用 **`MainBoard/` + `Module/` 分层架构**：

```
MainBoard/       # 业务层（手势流程、CNN 推理、main 入口、外设驱动）
Module/          # 可插拔功能模块（函数指针驱动的策略模式）
  ├── module0    # 红外遥控（发射 + 学习）
  └── module1~10 # 扩展槽位
```

调用方只需 `Module.Mode0_Handler()`，不关心底层是红外发射还是激光控制。
新增模块只写一个 `moduleX.c`，在 `CyberryPotter.c` 初始化中加一个 case 即可。

**扩展方向：**

- **更换传感器** — 将 `MPU6050.c` 中的驱动替换为 ICM-42688 / BMI160
- **替换红外控制** — 修改 `module0.c` 中的 `model_indices[]` 映射表适配不同家电
- **USB HID 手势鼠标** — 新增 `module11.c`，用 USB HID 协议发送鼠标坐标
- **更多手势** — 重新采集训练，修改 `CyberryPotter.h` 中的 `eModel_Output` 枚举和 `main.c` 中的输出循环

## 常见问题

**Q: 识别率不高怎么办？**
- 确保采集前法棒静止（1 秒校准窗口不能被跳过）
- 每个手势至少采集 50 组样本，动作尽量保持一致
- MPU6050 安装方向是否变化？重新校准或采集数据

**Q: 编译报错 "not enough space"?**
- STM32F103C8T6 只有 64KB Flash，确保勾选 Keil → Options → Target → IROM1 地址正确
- 改用 STM32F103RCT6（256KB Flash）有更大余量

**Q: 上位机打不开 / 闪退？**
- 确认 Python 3.7+ 环境，`pip install pyqt5 numpy pyserial tensorflow`
- 检查串口号和波特率（115200）

## 学习资源

- 📘 [CyberryPotter_学习指南.html](CyberryPotter_学习指南.html) — 按数据流组织的深度学习文档，含六个动手练习

## 许可与致谢

本项目基于 **NNOM**（[majianjia/nnom](https://github.com/majianjia/nnom)）推理框架开发，感谢作者的马工的开源贡献。

硬件方案参考了 "MiniBalance" 开源平衡车项目部分模块设计。

---

*Built with ❤️ on a 72MHz Cortex-M3 that has no business running a neural network.*

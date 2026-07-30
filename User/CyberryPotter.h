#include "config.h"

//********************************************************************************
#ifndef	_CYBERRY_POTTER_H_
#define	_CYBERRY_POTTER_H_
#include "hardware.h"
#include "Delay.h"
#include "IMU.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>


typedef enum eSystem_Mode{
        SYSTEM_GESTURE_IR = 0,   // 手势识别 + 红外发射 (默认)
        SYSTEM_IR_LEARN   = 1    // 红外学习模式
}eSystem_Mode;

typedef enum eButton_Status{
	BUTTON_IDLE = 0,
        BUTTON_RELEASE = 1,
        BUTTON_HOLD = 2,
	BUTTON_HOLD_LONG = 3,
	BUTTON_HOLD_VERY_LONG = 4
	
}eButton_Status;     

typedef enum eIMU_STATUS{
        IMU_Idle = 0,
        IMU_Sampling = 1,
        IMU_Sampled = 2,
	IMU_Sample = 3
}eIMU_STATUS;

typedef enum eLED_LED{
        LED_IDLE = 0,
        LED_5HZ = 1,
        LED_10HZ = 2,
	LED_ALWAYS_ON = 3
}eLED_STATUS;

typedef enum eSerial_Status{
        Serial_Idle = 0,
        Serial_Receiving = 1,
        Serial_Received = 2
}eSerial_Status;

// 红外信号状态 (从原版 CyberryPotter_status.h 搬移)
typedef enum eSignal_Status{
        SIGNAL_EMPTY     = 0,  // 位槽空，未学习
        SIGNAL_LOADED    = 1,  // 已从 Flash 加载码
        SIGNAL_RECORDING = 2,  // 学习中，等待外部遥控
        SIGNAL_RECORDED  = 3,  // 刚学完
        SIGNAL_SENDING   = 4,  // 正在发射
        SIGNAL_SENT      = 5   // 发射完成
}eSignal_Status;

typedef enum eSignal_Type{
        Signal_Type_None     = 0,
        Signal_Type_IR       = 1,
        Signal_Type_FR_433MHZ = 2
}eSignal_Type;

typedef struct Cyberry_Potter_Status_Typedef{
        eSystem_Mode System_Mode;
        eButton_Status Button_Status;
        uint8_t Button_Press_Count;
        uint8_t Button_Click_Count;
        eIMU_STATUS IMU_Status;
        eLED_STATUS LED_Status;
        eSerial_Status Serial_Status;
        // 红外信号管理
        eSignal_Status Signal_Status;
        eSignal_Type   Signal_Type;
        uint8_t        Ir_Learn_Slot;  // 当前选中的学习位槽 (0~12)
        uint8_t        Ir_Code_Addr;   // 当前手势对应的 IR 地址码
        uint8_t        Ir_Code_Cmd;    // 当前手势对应的 IR 命令码
}Cyberry_Potter_Status_Typedef;

void Cyberry_Potter_System_Status_Update(void);
void System_Init(void);
void Cyberry_Potter(void);
void LED_Blink(void);

#endif	//_CYBERRY_POTTER_H_

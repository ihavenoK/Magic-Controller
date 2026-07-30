#include "config.h"

#ifndef _IR_H_
#define _IR_H_

#include "stm32f10x.h"

// ---- RAW 信号存储 ----

#define IR_RAW_MAX_EDGES    (150)
#define IR_RAW_SLOT_SIZE    (1024)                  // 每手势 1024 字节 (1KB Flash 页对齐)
#define IR_GESTURE_COUNT    (13)
#define IR_FLASH_BASE       ((uint32_t)0x0801CC00)  // F103CBT6: pages 115-127 (13KB)

typedef struct {
    uint8_t  edge_count;     // 实际边沿数
    uint8_t  reserved[3];
    uint16_t edges[IR_RAW_MAX_EDGES];  // 微秒级脉冲宽度序列
} IR_Raw_Signal_t;

// ---- API ----

void IR_Init(void);

// 原始发射：按 edges 数组逐脉冲回放 PWM 载波
void IR_Send_Raw(const IR_Raw_Signal_t *sig);

// 阻塞等待 RAW 信号（TIM3 录制边沿），超时约 200ms，返回 1=成功
uint8_t IR_Receive_Raw(IR_Raw_Signal_t *sig);

// Flash 存储与读取
void IR_Flash_Save(uint8_t gesture_id, const IR_Raw_Signal_t *sig);
uint8_t IR_Flash_Load(uint8_t gesture_id, IR_Raw_Signal_t *sig);

// 保持兼容：获取手势对应码（空壳，RAW 模式不区分 addr/cmd）
uint8_t IR_Get_Gesture_Code(uint8_t gesture_id, uint8_t *addr);

#endif  // _IR_H_


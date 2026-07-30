/**
  * @file    IR.c
  * @brief   红外 RAW 万能学习模式 — 原始脉冲录制、回放、Flash 存储
  *          发射: TIM2_CH1 (PA15) 38kHz PWM
  *          接收: TIM3_CH3 (PB0) 下降沿输入捕获
  *          存储: Flash pages 122-127 (0x0801E800, 6KB)
  */

#include "IR.h"
#include "Delay.h"
#include "hardware.h"
#include <stdio.h>

/* ========== 硬件配置 ========== */

#define IR_PWM_PSC      (0)
#define IR_PWM_ARR      (1894)    // 38kHz
#define IR_PWM_PULSE    (632)     // 1/3 占空比

#define IR_CAP_PSC      (71)      // 1MHz → 1µs/tick
#define IR_CAP_ARR      (65535)

#define IR_RECV_TIMEOUT_MS  (200)

/* ========== RAW 接收缓冲区 ========== */

static volatile uint16_t raw_buf[IR_RAW_MAX_EDGES + 1];
static volatile uint8_t  raw_len  = 0;
static volatile uint8_t  raw_ready = 0;

/* ========== 内部函数 ========== */

static void ir_carrier_on(void)  { IR_CARRIER_ON(); }
static void ir_carrier_off(void) { IR_CARRIER_OFF(); }

/* ==================== IR_Init ==================== */

void IR_Init(void)
{
    GPIO_InitTypeDef  gpio;
    TIM_TimeBaseInitTypeDef tim_base;
    TIM_OCInitTypeDef tim_oc;
    TIM_ICInitTypeDef tim_ic;
    NVIC_InitTypeDef nvic;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

    // PA15: IR TX (TIM2_CH1, AF_PP)
    gpio.GPIO_Pin   = GPIO_Pin_15;
    gpio.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    // PB0: IR RX (TIM3_CH3, IPU)
    gpio.GPIO_Pin   = GPIO_Pin_0;
    gpio.GPIO_Mode  = GPIO_Mode_IPU;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    // TIM2: 38kHz PWM
    TIM_TimeBaseStructInit(&tim_base);
    tim_base.TIM_Prescaler     = IR_PWM_PSC;
    tim_base.TIM_Period        = IR_PWM_ARR;
    tim_base.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_base);

    TIM_OCStructInit(&tim_oc);
    tim_oc.TIM_OCMode      = TIM_OCMode_PWM1;
    tim_oc.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc.TIM_Pulse       = IR_PWM_PULSE;
    tim_oc.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &tim_oc);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    ir_carrier_off();

    // TIM3: 输入捕获 (下降沿)
    TIM_TimeBaseStructInit(&tim_base);
    tim_base.TIM_Prescaler     = IR_CAP_PSC;
    tim_base.TIM_Period        = IR_CAP_ARR;
    tim_base.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &tim_base);

    TIM_ICStructInit(&tim_ic);
    tim_ic.TIM_Channel     = TIM_Channel_3;
    tim_ic.TIM_ICPolarity  = TIM_ICPolarity_Falling;
    tim_ic.TIM_ICSelection = TIM_ICSelection_DirectTI;
    tim_ic.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    tim_ic.TIM_ICFilter    = 0x0F;
    TIM_ICInit(TIM3, &tim_ic);

    TIM_ITConfig(TIM3, TIM_IT_CC3 | TIM_IT_Update, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    nvic.NVIC_IRQChannel    = TIM3_IRQn;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority        = 0;
    NVIC_Init(&nvic);

    TIM_Cmd(TIM3, ENABLE);
}

/* ==================== RAW 发射 ==================== */

void IR_Send_Raw(const IR_Raw_Signal_t *sig)
{
    if (sig->edge_count < 2) return;

    for (uint8_t i = 0; i < sig->edge_count; i++) {
        if (i & 1) {
            // 奇数沿 = 载波 OFF
            ir_carrier_off();
        } else {
            // 偶数沿 = 载波 ON
            ir_carrier_on();
        }
        Delay_us(sig->edges[i]);
    }
    ir_carrier_off();
}

/* ==================== RAW 接收 ==================== */

uint8_t IR_Receive_Raw(IR_Raw_Signal_t *sig)
{
    uint32_t timeout = IR_RECV_TIMEOUT_MS;

    raw_len   = 0;
    raw_ready = 0;

    while (!raw_ready && timeout > 0) {
        Delay_ms(1);
        timeout--;
    }

    if (timeout == 0) return 0;

    sig->edge_count = raw_len;
    for (uint8_t i = 0; i < raw_len && i < IR_RAW_MAX_EDGES; i++) {
        sig->edges[i] = raw_buf[i];
    }
    return 1;
}

/* ==================== Flash 存储 ==================== */

void IR_Flash_Save(uint8_t gesture_id, const IR_Raw_Signal_t *sig)
{
    if (gesture_id >= IR_GESTURE_COUNT) return;

    // 每个 slot 独占 1 个 1KB 页 (1024 字节)
    uint32_t page_addr = IR_FLASH_BASE + gesture_id * IR_RAW_SLOT_SIZE;
    uint32_t *src = (uint32_t *)sig;
    uint32_t word_count = (sizeof(IR_Raw_Signal_t) + 3) / 4;

    FLASH_Unlock();
    FLASH_ErasePage(page_addr); // 擦除属于自己的这 1 页，绝不破坏其他槽位

    for (uint32_t i = 0; i < word_count; i++) {
        FLASH_ProgramWord(page_addr + i * 4, src[i]);
    }
    FLASH_Lock();
}

uint8_t IR_Flash_Load(uint8_t gesture_id, IR_Raw_Signal_t *sig)
{
    if (gesture_id >= IR_GESTURE_COUNT) return 0;

    uint32_t page_addr = IR_FLASH_BASE + gesture_id * IR_RAW_SLOT_SIZE;
    const uint32_t *src = (const uint32_t *)page_addr;

    // 擦除态检查
    if (src[0] == 0xFFFFFFFF) return 0;

    uint32_t *dst = (uint32_t *)sig;
    uint32_t word_count = (sizeof(IR_Raw_Signal_t) + 3) / 4;
    for (uint32_t i = 0; i < word_count; i++) {
        dst[i] = src[i];
    }

    if (sig->edge_count < 3 || sig->edge_count > IR_RAW_MAX_EDGES) return 0;
    return 1;
}

/* ==================== 兼容桩 ==================== */

uint8_t IR_Get_Gesture_Code(uint8_t gesture_id, uint8_t *addr)
{
    // RAW 模式不再用 addr/cmd 对
    if (addr) *addr = 0x00;
    return 0x00;
}

/* ==================== TIM3 中断：RAW 双边沿脉冲录制 ==================== */

void TIM3_IRQHandler(void)
{
    static uint16_t last_cap    = 0;
    static uint8_t  first_edge  = 1;
    static uint8_t  timeout_cnt = 0;

    uint16_t cap_val;
    uint16_t pulse_width;

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        // 静默超时判断：连续 2 次 TIM3 溢出 (约 130ms 静默) 说明遥控信号已发送完毕
        if (!first_edge) {
            timeout_cnt++;
            if (timeout_cnt >= 2) {
                if (raw_len >= 3) {
                    raw_ready = 1;
                }
                first_edge  = 1;
                timeout_cnt = 0;
                // 复位极性为下降沿敏感，等待下一次信号
                TIM3->CCER |= TIM_CCER_CC3P;
            }
        }
        return;
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC3) == RESET) return;

    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
    cap_val = TIM_GetCapture3(TIM3);
    timeout_cnt = 0; // 收到边沿，清零静默计数

    if (first_edge) {
        // 第 1 个边沿（下降沿：光亮开始），记录起始时间
        last_cap   = cap_val;
        first_edge = 0;
        raw_len    = 0;
        // 切换极性为上升沿敏感（光灭）
        TIM3->CCER &= ~TIM_CCER_CC3P;
        return;
    }

    // 计算与上一边沿的时间差 (µs)
    pulse_width = (cap_val >= last_cap) ? (cap_val - last_cap)
                                         : (0xFFFF - last_cap + cap_val);
    last_cap = cap_val;

    if (raw_len < IR_RAW_MAX_EDGES) {
        raw_buf[raw_len++] = pulse_width;
        // 动态交替翻转极性：下降沿 ↔ 上升沿
        TIM3->CCER ^= TIM_CCER_CC3P;
    }
}


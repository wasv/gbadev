#pragma once

#include "bitwise.h"
#include "memmap.h"

#define IRQ_VBL_SHIFT 0
#define IRQ_HBL_SHIFT 1
#define IRQ_VCT_SHIFT 2

#define IRQ_TM0_SHIFT 3
#define IRQ_TM1_SHIFT 4
#define IRQ_TM2_SHIFT 5
#define IRQ_TM3_SHIFT 6

#define IRQ_SER_SHIFT 7

#define IRQ_DM0_SHIFT 8
#define IRQ_DM1_SHIFT 9
#define IRQ_DM2_SHIFT 10
#define IRQ_DM3_SHIFT 11

#define IRQ_KEY_SHIFT 12
#define IRQ_GPK_SHIFT 13

#define TEST_IRQ(name) (BIT_EQ(REG_IF, BIT(IRQ_##name##_SHIFT)))
#define IRQ_ENABLE(name) (BIT_SET(REG_IE, BIT(IRQ_##name##_SHIFT)))
#define IRQ_DISABLE(name) (BIT_CLEAR(REG_IE, BIT(IRQ_##name##_SHIFT)))
#define IME_SET(enabled) (REG_IME = enabled & BIT(0))

typedef void isr_t(void);

void ISR(void) __attribute__((section(".iwram"), target("arm")));

void ISR_VBlank(void);
void ISR_HBlank(void);
void ISR_VCount(void);

void ISR_Timer0(void);
void ISR_Timer1(void);
void ISR_Timer2(void);
void ISR_Timer3(void);

void ISR_Serial(void);

void ISR_DMA0(void);
void ISR_DMA1(void);
void ISR_DMA2(void);
void ISR_DMA3(void);

void ISR_Keys(void);
void ISR_Game(void);

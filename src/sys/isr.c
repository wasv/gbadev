#include "isr.h"

void _stall() {
    while (1)
        ;
}

void ISR_VBlank(void) __attribute__((weak, alias("_stall")));
void ISR_HBlank(void) __attribute__((weak, alias("_stall")));
void ISR_VCount(void) __attribute__((weak, alias("_stall")));

void ISR_Timer0(void) __attribute__((weak, alias("_stall")));
void ISR_Timer1(void) __attribute__((weak, alias("_stall")));
void ISR_Timer2(void) __attribute__((weak, alias("_stall")));
void ISR_Timer3(void) __attribute__((weak, alias("_stall")));

void ISR_Serial(void) __attribute__((weak, alias("_stall")));

void ISR_DMA0(void) __attribute__((weak, alias("_stall")));
void ISR_DMA1(void) __attribute__((weak, alias("_stall")));
void ISR_DMA2(void) __attribute__((weak, alias("_stall")));
void ISR_DMA3(void) __attribute__((weak, alias("_stall")));

void ISR_Keys(void) __attribute__((weak, alias("_stall")));
void ISR_Game(void) __attribute__((weak, alias("_stall")));

void ISR(void) {
    isr_t *routine = _stall;

    if (TEST_IRQ(VBL)) {
        routine = &ISR_VBlank;
    } else if (TEST_IRQ(HBL)) {
        routine = &ISR_HBlank;
    } else if (TEST_IRQ(VCT)) {
        routine = &ISR_VCount;
    } else if (TEST_IRQ(TM0)) {
        routine = &ISR_Timer0;
    } else if (TEST_IRQ(TM1)) {
        routine = &ISR_Timer1;
    } else if (TEST_IRQ(TM2)) {
        routine = &ISR_Timer2;
    } else if (TEST_IRQ(TM3)) {
        routine = &ISR_Timer3;
    } else if (TEST_IRQ(SER)) {
        routine = &ISR_Serial;
    } else if (TEST_IRQ(DM0)) {
        routine = &ISR_DMA0;
    } else if (TEST_IRQ(DM1)) {
        routine = &ISR_DMA1;
    } else if (TEST_IRQ(DM2)) {
        routine = &ISR_DMA2;
    } else if (TEST_IRQ(DM3)) {
        routine = &ISR_DMA3;
    } else if (TEST_IRQ(KEY)) {
        routine = &ISR_Keys;
    } else if (TEST_IRQ(GPK)) {
        routine = &ISR_Game;
    }

    routine();
    REG_IF |= REG_IF;
}

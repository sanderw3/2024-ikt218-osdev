#pragma once
#include "Interrupts/ISR/ISR.h"


#define REMAP_OFFSET_MASTER_PIC     0x20
#define REMAP_OFFSET_SLAVE_PIC      REMAP_OFFSET_MASTER_PIC + 8


typedef void (*irq_handler)(Interrupt_Info* info);

void IRQinitiate();
void register_irq_handler(int irq, irq_handler handler);
void timerIRQHandler(Interrupt_Info* info);
    
#include "Interrupts/IRQ/IRQ.h"
#include "Interrupts/PIC/PIC.h"
#include "Interrupts/ISR/ISR.h"
#include "Interrupts/IO/IO.h"
#include "libc/stddef.h"
#include "libc/stdio.h"
#include "Drivers/keyboardIRQ.h"



irq_handler irq_handlers[16];

void defaultIRQHandler(Interrupt_Info* info) {
    int irqNum = info->interrupt_number - REMAP_OFFSET_MASTER_PIC;

    uint8_t picISR = RetrieveISRRegisters();
    uint8_t picIRR = RetrieveIRQRegisters();
    
    if (irq_handlers[irqNum] == NULL) {
        printf("Unhandled IRQ: %d, %d, %d\n", irqNum, picISR, picIRR);
    } else {
        // call any irq handlers added by "register_irq_handler"
        irq_handlers[irqNum](info);
    }

    // send end of interrupt
    end_of_interrupt(irqNum);
} 

void IRQinitiate() {
    initialize_pic(REMAP_OFFSET_MASTER_PIC, REMAP_OFFSET_SLAVE_PIC);

    for (int i = 0; i < 16; i++) {
        // for each irq, we change the default isr to the new default irq handler:
        AddInterruptHandler(REMAP_OFFSET_MASTER_PIC + i, defaultIRQHandler);
    }

    // to load specific created handlers
    // timer handler (PIT):
    // mask_pic(0); // we disable the timer for now
    register_irq_handler(0, timerIRQHandler);

    // keyboard handler:
    register_irq_handler(1, keyboardIRQHandler);

}

void register_irq_handler(int irqNum, irq_handler handler) {
    irq_handlers[irqNum] = handler;
}


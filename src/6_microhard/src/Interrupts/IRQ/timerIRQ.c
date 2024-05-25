#include "Interrupts/IRQ/IRQ.h"
#include "Interrupts/IRQ/timerIRQ.h"
#include "Interrupts/PIT/PIT.h"
#include "Interrupts/IO/IO.h"

uint32_t time = 0;

void timerIRQHandler(Interrupt_Info* info) {
    // increment ms on every tick (since the tick rate is the same as a ms)
    time++;
}

uint32_t millis(){
    return time;
}
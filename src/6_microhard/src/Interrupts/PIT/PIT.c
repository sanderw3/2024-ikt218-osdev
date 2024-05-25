#include "Interrupts/PIT/PIT.h"
#include "Interrupts/IRQ/timerIRQ.h"
#include "Interrupts/IO/IO.h"


void init_pit(){
    // Set the PIT frequency to 100 Hz (10 ms interval)
    outputb(PIT_CMD_PORT, 0x36); // Command byte: Channel 0, Mode 3, Binary mode

    outputb(0x40, DIVIDER & 0xFF); // Low byte of divisor
    outputb(0x40, (DIVIDER >> 8) & 0xFF); // High byte of divisor

    return;
}



// this function waits by using interrupts, which lets the cpu do other things while waiting
void sleep_interrupt(uint32_t milliseconds){
    uint32_t current_tick = millis();       
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS; // Define TICKS_PER_MS as the number of clock ticks per millisecond
    uint32_t end_ticks = current_tick + ticks_to_wait;

    while (current_tick < end_ticks) {
        asm volatile("sti"); // Enable interrupts
        asm volatile("hlt"); // Halt the CPU until the next interrupt
        current_tick = millis();
    }
}



// this function waits by using busy waiting, which uses a the cpu time purly to wait
void sleep_busy(uint32_t milliseconds){
    uint32_t start_tick = millis();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS; // Define TICKS_PER_MS as the number of clock ticks per millisecond
    uint32_t elapsed_ticks = 0;

    while (elapsed_ticks < ticks_to_wait) {
        while (millis() == start_tick + elapsed_ticks) {
            // Busy wait
        }
        elapsed_ticks++;
    }
}



// this function gets the current tick from the PIT (this current tick reloads when PIT reaches divisor value)
uint32_t get_current_tick(){
    uint32_t tick = 0;

    // to correctly read the data we temporarily stop the interrupts
    DisableInterrupts();

    outputb(PIT_CMD_PORT, 0x00); // Latch the current count
    tick = inputb(PIT_CHANNEL0_PORT); // Read the low byte
    tick |= inputb(PIT_CHANNEL0_PORT) << 8; // Read the high byte

    // then we enable them again
    EnableInterrupts();
    return tick;
}
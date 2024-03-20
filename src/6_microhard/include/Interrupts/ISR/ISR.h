#pragma once
#include "libc/stdint.h"
#include "Interrupts/IDT/IDT.h"


// this struct is used to store the values passed by the ISR_cpu_state_capture function in ISR.asm
// all of these values are retrieved by placing them in the reverse order of how they are pushed to the stack
typedef struct {
    // the data segment (same as the other segments)
    uint32_t data_segment;

    // these are all the registers pushed by pusha
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;


    // this is pushed by the cpu and/or the ISR 
    uint32_t interrupt_number;
    uint32_t error_code;


    // this is pushed by the cpu at the time of the interrupt
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp_at_signal;
    uint32_t ss;
    
} __attribute__((packed)) Interrupt_Info;



// this is a handler for all the interrupts. 
// It will print the interrupt number and the error code of the current interrupt
// before it calls a more specified handler for specific
void InterruptHandler(Interrupt_Info* info);

// table with all the IRS from assembly
// this is not really necessary but it require less lines of code
// instead we could have just manually written all 256 isr declared in assembly, in the IDTsetgate function.
extern void* ISR_ERROR_TABLE[];


// macros to add an interrupt to the ISR table
typedef void (*isrSecifiedHandler)(Interrupt_Info* info);

void AddInterruptHandler(int interrupt, isrSecifiedHandler handler); // adds an Interrupt
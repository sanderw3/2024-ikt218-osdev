#include "Interrupts/ISR/ISR.h"
#include "libc/stdio.h"
#include "libc/stddef.h"
#include "Interrupts/IO/IO.h"

// this is the table that contains all the functions we implement for each interrupt
isrSecifiedHandler ISR_SPECIFIED_HANDLERS[IDT_SIZE];


// from: https://wiki.osdev.org/Exceptions#Double_Fault
// all the processor generated exceptions are defined here
const char* generatedExceptions[32] = {
    "Divide by zero error",                 // 0x00
    "Debug",                                // 0x01
    "Non-maskable interrupt",               // 0x02
    "Breakpoint",                           // 0x03
    "Overflow",                             // 0x04
    "Bound range exceeded",                 // 0x05
    "Invalid opcode",                       // 0x06
    "Device not available",                 // 0x07
    "Double fault",                         // 0x08
    "Coprocessor segment overrun",          // 0x09
    "Invalid TSS",                          // 0x0A
    "Segment not present",                  // 0x0B
    "Stack fault",                          // 0x0C
    "General protection fault",             // 0x0D
    "Page fault",                           // 0x0E
    "Reserved",                             // 0x0F
    "Coprocessor error",                    // 0x10
    "Alignment check",                      // 0x11
    "Machine check",                        // 0x12
    "SIMD floating point",                  // 0x13                
    "Virtualization",                       // 0x14
    "Control protection",                   // 0x15
    "Reserved",                             // 0x16
    "Reserved",                             // 0x17
    "Reserved",                             // 0x18
    "Reserved",                             // 0x19
    "Reserved",                             // 0x1A
    "Reserved",                             // 0x1B
    "Hypervisor injection",                 // 0x1C
    "VMM communication",                    // 0x1D
    "Security",                             // 0x1E
    "Reserved",                             // 0x1F
};


// this is to handle any interrupts which may occure
// prints information about the interrupt
void InterruptHandler(Interrupt_Info* info){
    
    // then we call the more specific handler if there is one
    if (ISR_SPECIFIED_HANDLERS[info->interrupt_number] != NULL){
        ISR_SPECIFIED_HANDLERS[info->interrupt_number](info);
    } else {
        if (info->interrupt_number < 32){ // < 0x20
            printf("Exception: %s\n", generatedExceptions[info->interrupt_number]);
            // disable interrupts ("cli" instruction)
            DisableInterrupts();

            // halt the system
            __asm__ volatile("hlt");
        }
        
        printf("No handler for interrupt\n");
        
    }
}



// to add an interrupt to the ISR table
void AddInterruptHandler(int interrupt, isrSecifiedHandler handler){
    ISR_SPECIFIED_HANDLERS[interrupt] = handler;     
    EnableIDTgate(interrupt);
}

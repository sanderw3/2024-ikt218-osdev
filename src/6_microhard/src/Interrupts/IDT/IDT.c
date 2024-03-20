#include "Interrupts/IDT/IDT.h"
#include "Interrupts/ISR/ISR.h"
#include "libc/stdio.h"
#include "libc/stddef.h"

__attribute__((aligned(0x10)))                              // aligned for performance
IDTEntry idt[IDT_SIZE];                                     // the idt table itself 
IDTRegister idtp = {(uint16_t)(sizeof(idt) - 1), &idt[0]};  // the size of the idt table andthe address to the start of the table



// this is the helperfunction to set the idt entry
// like done described here: https://wiki.osdev.org/Interrupts_Tutorial
void IDTsetgate(uint8_t num, uint32_t start, uint16_t sel, uint8_t flags){
    IDTEntry* entry = &idt[num];
    entry->lower_offset = start & 0xFFFF;                           
    entry->upper_offset = (start >> 16) & 0xFFFF;
    entry->selector = sel;
    entry->zero = 0;
    entry->flags = flags;
}



void IDTinitiate(){

    for (unsigned short i = 0; i < IDT_SIZE; i++){                  // loops through 0 - 255 (256) entries
        IDTsetgate(i,                                               // sets the idt entry number (interrupt number)
            (uint32_t)ISR_ERROR_TABLE[i],                           // sets the interrupt handler
            0x08,                                                   // sets the selector to the code segment
            PRESENT | INTERRUPT_GATE_32BIT | PRIVILEGE_LEVEL_0);    // sets the flags (1000 1110 / 0x8E)
    }    
    IDTload();                                                      // loads the idt (function in assembly)
}


void EnableIDTgate(uint32_t num){
    idt[num].flags |= PRESENT;             // sets the left most bit to 1 (OR operation with 0x80)
}                                          // which sets the present flag to 1

void DisableIDTgate(uint32_t num){
    idt[num].flags &= ~PRESENT;            // sets the left most bit to 0 (AND operation with not 0x80 (0x7F)) 
}                                          // which sets the present flag to 0

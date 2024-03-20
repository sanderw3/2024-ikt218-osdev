#pragma once
#include "libc/stdint.h"


#define IDT_SIZE 256

//flags: 
// first 4 bits of the flag:
#define TASK_GATE               0x05        // for hardware multitasking 
#define INTERRUPT_GATE_16BIT    0x06        // current instruction is skipped (next instruction is saved)
#define TRAP_GATE_16BIT         0x07        // current instruction is meant to be retried (instruction is saved)
#define INTERRUPT_GATE_32BIT    0x0E        // current instruction is skipped (next instruction is saved)
#define TRAP_GATE_32BIT         0x0F        // current instruction is meant to be retried (instruction is saved)

// three next bit of the flag:              // to set the privlage level of the interrupt
#define PRIVILEGE_LEVEL_0       0x00        // 0 = highest
#define PRIVILEGE_LEVEL_1       0x10        // 1 
#define PRIVILEGE_LEVEL_2       0x40        // 2 
#define PRIVILEGE_LEVEL_3       0x60        // 3 = lowest

// last bit of the flag:    
#define PRESENT                 0x80        // the interrupt is present (enabled)


// the idt entry structure
typedef struct {
    uint16_t lower_offset;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t upper_offset;

} __attribute__((packed)) IDTEntry;


// the idt register
typedef struct {
    uint16_t limit;
    IDTEntry* address;
} __attribute__((packed)) IDTRegister;


extern void IDTload(); 
void IDTinitiate();
void IDTsetgate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);


// these functions can be used to enable or disable a specific interrupt if needed
void EnableIDTgate(uint32_t num);
void DisableIDTgate(uint32_t num);






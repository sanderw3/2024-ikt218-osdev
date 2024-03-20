#include "GDT/GDT.h"
#include "libc/stdint.h"

static GDTentry gdt[GDT_SIZE];   // instance to store 3 GDT entries
GDTdescriptor gp;                // to store the address and limit of the GDT 
int32_t gdt_count = 0;           // to count the number of GDT entries

void GDTsetgate(size_t base, size_t limit, uint8_t access, uint8_t gran){

    // to setup the base address of the descriptor:
    gdt[gdt_count].base_low = (base & 0xFFFF);
    gdt[gdt_count].base_middle = (base >> 16) & 0xFF;
    gdt[gdt_count].base_high = (base >> 24) & 0xFF;

    // descriptor limits
    gdt[gdt_count].limit_low = (limit & 0xFFFF);
    gdt[gdt_count].granularity = ((limit >> 16) & 0x0F);

    // descriptor flags
    gdt[gdt_count].access = access;
    gdt[gdt_count].granularity |= (gran & 0xF0);
    

    // increment the number of GDT entries
    gdt_count++;
}


void GDTinitiate(){

    // create GDT pointer
    gp.limit = (sizeof(GDTentry) * 3) - 1;
    gp.base = &gdt; 

    // this is our entires to the GDT table
    // null descriptor
    GDTsetgate(0, 0, 0, 0);                  
    

    // code segment
    GDTsetgate(
        0x00,

        FULL_32_BIT_RANGE, 

        // access field                            // bits are written in order: top -> bottom = right -> left
        SEGMENT_ACCESSED(0)             |          // 0 handled by the CPU
        SEGMENT_READ_AND_WRITE(1)       |          // 1 can both read and execute
        SEGMENT_CONF_AND_DIR(0)         |          // 0 normal direction
        SEGMENT_EXECUTABLE(1)           |          // 1 executable = code segment
        SEGMENT_DESCRIPTOR_TYPE(1)      |          // 1 data/code segment (not system)
        SEGMENT_PRIVLAGE(0)             |          // 00 = highest, 11 = lowest
        SEGMENT_PRESENT(1),                        // 1 = present
        
        // granularity field
        SEGMENT_LIMIT(0XF)              |          // 1111 max limit (4GB)
        SEGMENT_AVAILABILITY(0)         |          // 0 ignored by the cpu (for operativsystem)
        SEGMENT_64BIT(0)                |          // 0 for 16/32-bit, 1 for 64-bit
        SEGMENT_GRANULARITY(1)          |          // 1 granularity (4KB - 4GB) segment limits
        SEGMENT_SIZE(1)                            // 0 for 16bit, 1 for 32bit
    );            


    // data segment
    GDTsetgate(
        0x00,

        FULL_32_BIT_RANGE,

        // access field 
        SEGMENT_ACCESSED(0)             |          // 0 handled by the CPU
        SEGMENT_READ_AND_WRITE(1)       |          // 1 can both read and execute
        SEGMENT_CONF_AND_DIR(0)         |          // 0 normal direction
        SEGMENT_EXECUTABLE(0)           |          // 0 executable = data segment
        SEGMENT_DESCRIPTOR_TYPE(1)      |          // 1 data/code segment (not system)
        SEGMENT_PRIVLAGE(0)             |          // 00 = highest, 11 = lowest
        SEGMENT_PRESENT(1),                        // 1 = present

        // granularity field
        SEGMENT_LIMIT(0XF)              |          // 1111 max limit (4GB)
        SEGMENT_AVAILABILITY(0)         |          // 0 ignored by the cpu (for operativsystem)
        SEGMENT_64BIT(0)                |          // 0 for 16/32-bit, 1 for 64-bit
        SEGMENT_GRANULARITY(1)          |          // 1 granularity (4KB - 4GB) segment limits
        SEGMENT_SIZE(1)                            // 0 for 16bit, 1 for 32bit    
    );              


    // user mode code segment
    GDTsetgate(
        0x00,

        FULL_32_BIT_RANGE,

        // access field 
        SEGMENT_ACCESSED(0)             |          // 0 handled by the CPU
        SEGMENT_READ_AND_WRITE(1)       |          // 1 can both read and execute
        SEGMENT_CONF_AND_DIR(0)         |          // 0 normal direction
        SEGMENT_EXECUTABLE(1)           |          // 1 executable = code segment
        SEGMENT_DESCRIPTOR_TYPE(1)      |          // 1 data/code segment (not system)
        SEGMENT_PRIVLAGE(3)             |          // 00(0) = highest, 11(3) = lowest
        SEGMENT_PRESENT(1),                        // 1 = present

        // granularity field
        SEGMENT_LIMIT(0XF)              |          // 1111 max limit (4GB)
        SEGMENT_AVAILABILITY(0)         |          // 0 ignored by the cpu (for operativsystem)
        SEGMENT_64BIT(0)                |          // 0 for 16/32-bit, 1 for 64-bit
        SEGMENT_GRANULARITY(1)          |          // 1 granularity (4KB - 4GB) segment limits
        SEGMENT_SIZE(1)                            // 0 for 16bit, 1 for 32bit    
    );         


    // user mode data segment
    GDTsetgate(
        0x00,

        FULL_32_BIT_RANGE,

        // access field 
        SEGMENT_ACCESSED(0)             |          // 0 handled by the CPU
        SEGMENT_READ_AND_WRITE(1)       |          // 1 can both read and execute
        SEGMENT_CONF_AND_DIR(0)         |          // 0 normal direction
        SEGMENT_EXECUTABLE(0)           |          // 0 executable = data segment
        SEGMENT_DESCRIPTOR_TYPE(1)      |          // 1 data/code segment (not system)
        SEGMENT_PRIVLAGE(3)             |          // 00 = highest, 11 = lowest
        SEGMENT_PRESENT(1),                        // 1 = present

        // granularity field
        SEGMENT_LIMIT(0XF)              |          // 1111 max limit (4GB)
        SEGMENT_AVAILABILITY(0)         |          // 0 ignored by the cpu (for operativsystem)
        SEGMENT_64BIT(0)                |          // 0 for 16/32-bit, 1 for 64-bit
        SEGMENT_GRANULARITY(1)          |          // 1 granularity (4KB - 4GB) segment limits
        SEGMENT_SIZE(1)                            // 0 for 16bit, 1 for 32bit    
    );         


    // loads and flushes the GDT 
    GDTload();
}


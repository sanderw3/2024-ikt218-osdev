#pragma once
#include "libc/stdint.h"

#define FULL_32_BIT_RANGE                   0xFFFFFFFF  // 32 bit (only the first 0xFFFFF is used)
#define GDT_SIZE                            0x05        // 5 entries

// based off of: https://wiki.osdev.org/GDT_Tutorial
// set Access field bits:
#define SEGMENT_ACCESSED(x)             ((x) << 0x00)                  // Accessed (0 for not accessed, 1 for accessed)
#define SEGMENT_READ_AND_WRITE(x)       ((x) << 0x01)                  // Data: Read and write (0 for read only, 1 for read and write)    Code: (0 for executable only, 1 for executable and read)
#define SEGMENT_CONF_AND_DIR(x)         ((x) << 0x02)                  // Data: Conforming (0 for not conforming, 1 for conforming)       Code: (0 for normal direction, 1 for reverse direction)
#define SEGMENT_EXECUTABLE(x)           ((x) << 0x03)                  // Executable (0 for data segment, 1 for code segment)
#define SEGMENT_DESCRIPTOR_TYPE(x)      ((x) << 0x04)                  // Descriptor type (0 for system, 1 for code/data)
#define SEGMENT_PRIVLAGE(x)             (((x) &  0x03) << 0x05)        // Set privilege level (0 - 3) 0 = highest, 3 = lowest
#define SEGMENT_PRESENT(x)              ((x) << 0x07)                  // Set present (0 for not present, 1 for present)

// set Granularity field bits
#define SEGMENT_LIMIT(x)                (((x) & 0xF) << 0x00)          // Set segment limit (in bytes if granularity is 0, in pages (4kb) if granularity is 1)
#define SEGMENT_AVAILABILITY(x)         ((x) << 0x04)                  // Set available (0 for not available, 1 for available)
#define SEGMENT_64BIT(x)                ((x) << 0x05)                  // Set 64-bit (0 for 16/32-bit, 1 for 64-bit) (LONG) (segment operand size must then be 0 if this bit is set)
#define SEGMENT_SIZE(x)                 ((x) << 0x06)                  // Set segment size (0 for 16bit, 1 for 32bit)
#define SEGMENT_GRANULARITY(x)          ((x) << 0x07)                  // Set granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)



typedef struct
{
    uint16_t limit_low;             // this specifies the size of the segment
    uint16_t base_low;              // this specifies the starting address of the segment
    uint8_t base_middle;            // this is for the next 8 bits of the base field
    uint8_t access;                 // for access flags
    uint8_t granularity;            // granularity flags
    uint8_t base_high;              // the highest 8 bits of the base field
} __attribute__((packed)) GDTentry; // __attribute__((packed)) is to tell the compiler to keep the compiled code to the minimum (least possible padding)


typedef struct 
{
    uint16_t limit;                 // defines the size of the GDT - 1
    GDTentry* base;                 // base address where the GDT is located
}__attribute__((packed)) GDTdescriptor;


extern void GDTload();             // loads and flushes the GDT 

 
void GDTsetgate(size_t base, size_t limit, uint8_t access, uint8_t gran);
void GDTinitiate();

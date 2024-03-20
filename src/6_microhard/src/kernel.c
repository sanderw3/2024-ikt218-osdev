#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "multiboot2.h"
#include "GDT/GDT.h"
#include "Interrupts/IDT/IDT.h"
#include "Interrupts/IRQ/IRQ.h"
#include "Interrupts/IO/IO.h"

void timer(Interrupt_Info* info) {
    printf(".");
}

void initiate() {
    // task 2
    GDTinitiate();

    // task 3
    IDTinitiate();
    IRQinitiate();
    register_irq_handler(0, timer);
    EnableInterrupts();
}

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    initiate();
    Set_text_color(LIGHTGRAY);
    printf("Deez nutz: %.5f \n", 3223.141596767699979797); 
  
    // printf("test1\n");

    // // test interrupts
    // __asm__ volatile ("int $0x01");
    // __asm__ volatile ("int $0x02");
    // __asm__ volatile ("int $0x00");

    // printf("test33\n");

    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}
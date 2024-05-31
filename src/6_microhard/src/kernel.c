#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "multiboot2.h"
#include "GDT/GDT.h"
#include "Interrupts/IDT/IDT.h"
#include "Interrupts/IRQ/IRQ.h"
#include "Interrupts/IO/IO.h"
#include "Interrupts/PIT/PIT.h"
#include "Interrupts/PIC/PIC.h"
#include "Memory/memory.h"
#include "Drivers/display.h"




extern uint32_t end;

void initiate() {
    // task 2
    GDTinitiate();

    // task 3
    IDTinitiate();
    IRQinitiate();
    EnableInterrupts();
    mask_pic(1);

    // task 4
    init_kernel_memory(&end);
    init_paging();
    init_pit(); // <- part 2

    print_memory_layout(true);
}

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    setUnderlineCursor();

    //initialize
    initiate();
    clearScreen();
    Set_text_color(RED);
    printSplashScreen();
    Set_text_color(DEFAULT);
    unmask_pic(1);

    // printf("test: %s", "command");
  
    // printf("test1\n");

    // // test interrupts
     // __asm__ volatile ("int $0x01");
    // __asm__ volatile ("int $0x02");
    // __asm__ volatile ("int $0x00");

    // printf("test %i\n", 20/0);


    
    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}
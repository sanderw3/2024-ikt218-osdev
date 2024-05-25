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
#include "Memory/memory.h"

extern uint32_t end;

void initiate() {
    // task 2
    GDTinitiate();

    // task 3
    IDTinitiate();
    IRQinitiate();
    EnableInterrupts();

    // task 4
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit(); // <- part 2

    // task 5
}

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    //initialize
    initiate();
    Set_text_color(LIGHTGRAY);
    // // printf("Deez nutz: %.5f \n", 3223.141596767699979797); 
    // printf("███╗   ███╗██╗ ██████╗██████╗  ██████╗ ██╗  ██╗ █████╗ ██████╗ ██████╗ \n"
    //        "████╗ ████║██║██╔════╝██╔══██╗██╔═══██╗██║  ██║██╔══██╗██╔══██╗██╔══██╗\n"
    //        "██╔████╔██║██║██║     ██████╔╝██║   ██║███████║███████║██████╔╝██║  ██║\n"
    //        "██║╚██╔╝██║██║██║     ██╔══██╗██║   ██║██╔══██║██╔══██║██╔══██╗██║  ██║\n"
    //        "██║ ╚═╝ ██║██║╚██████╗██║  ██║╚██████╔╝██║  ██║██║  ██║██║  ██║██████╔╝\n"
    //        "╚═╝     ╚═╝╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ \n");

//  printf(
//             "##     ###### ###### ########  ####### ##     ##   ###   ######## ######## \n"
//             "###   ### ## ##    ####     ####     ####     ##  ## ##  ##     ####     ##\n"
//             "#### #### ## ##      ##     ####     ####     ## ##   ## ##     ####     ##\n"
//             "## ### ## ## ##      ######## ##     #############     ########## ##     ##\n"
//             "##     ## ## ##      ##   ##  ##     ####     #############   ##  ##     ##\n"
//             "##     ## ## ##    ####    ## ##     ####     ####     ####    ## ##     ##\n"
//             "##     ###### ###### ##     ## ####### ##     ####     ####     ########## \n");
    // printf("                                    _                       _ \n");
    // printf("            _                      ( )                     ( )\n");
    // printf("  ___ ___  (_)   ___  _ __    _    | |__     _ _  _ __    _| |\n");
    // printf("/' _ ` _ `\\| | /'___)( '__)/'_`\\ |  _ `\\ /'_` )( '__)/'_` |\n");
    // printf("| ( ) ( ) || |( (___ | |   ( (_) )| | | |( (_| || |   ( (_| |\n");
    // printf("(_) (_) (_)(_)`\\____)(_)   `\\___/'(_) (_)`\\__,_)(_)   `\\__,_)\n");
   
    

    
  
    // printf("test1\n");

    // // test interrupts
    // __asm__ volatile ("int $0x01");
    // __asm__ volatile ("int $0x02");
    // __asm__ volatile ("int $0x00");

    // printf("test33\n");

    
    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}
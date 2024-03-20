[bits 32]
;even though this can be done from one line in C, it's more readable like this
; in c:     asm("lidt %0"::"m"(idtp));



global IDTload                ;the IDTload from IDT.h

extern idtp                   ;the IDT from IDT.c
section .text
IDTload:
    lidt [idtp]               ; load IDT from idt
    ret                       ; return to the c code






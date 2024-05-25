[bits 32]

; like done in http://www.osdever.net/bkerndev/Docs/gdt.htm
global GDTload                ;the GDTload from GDT.c

extern gp
section .text
GDTload:
    lgdt [gp]                  ; load GDT from gp 
    mov ax, 0x10               ; 0x10 = 16 bit = offset to data segment
    mov ds, ax                 ; data segment
    mov es, ax                 ; extra segment
    mov fs, ax                 ; file segment
    mov gs, ax                 ; general segment
    mov ss, ax                 ; stack segment
    jmp 0x08:.flush2           ; 0x08 = 8 bit = offset to the code segment + the offset to the flush2 label 
.flush2:                        
    ret                        ; return to the c code









[bits 32]

extern InterruptHandler

; function that handle the interrupts with error codes
%macro ISR_WITH_ERR 1
global ISR_WITH_ERR_%1:
ISR_WITH_ERR_%1:
    push %1                                         ; pushes that number in the stack
    jmp ISR_cpu_state_capture                       ; captures the state of the cpu at the time of the interrupt
%endmacro


; function that handle the interrupts without error codes
%macro ISR_WITHOUT_ERR 1  
global ISR_WITHOUT_ERR_%1:       
ISR_WITHOUT_ERR_%1:                                 ; %1 changes with the interrupt number specified in c
    push 0                                          ; pushes 0 in the stack (as a placeholder for no errors)
    push %1                                         ; pushes that number in the stack
    jmp ISR_cpu_state_capture                       ; captures the state of the cpu at the time of the interrupt
%endmacro



; OBS: if fs, es and gs does not have the same value: push and pop them aswell
ISR_cpu_state_capture:
    pusha                                           ; push all registers in the order: eax, ecx, edx, ebx, esp, ebp, esi, edi
    xor eax, eax                                    ; clears the eax register ( set it to 0 )
    mov ax, ds                                      ; moves the data segment to ax (lower 16 bits of eax) (fs, es and gs have the same value)
    push eax                                        ; pushes the now 32-bit value of ds to the stack

    
    mov ax, 0x10                                    ; sets the segments to the kernel data segment so the exception handler is safe to execute
    mov ds, ax                                      ; 
    mov fs, ax                                      ; 
    mov es, ax                                      ; 
    mov gs, ax                                      ; 
    push esp                                        ; push the stack pointer

    call InterruptHandler                           ; call the function in c to handle said interrupt
    add esp, 4                                      ; removes the stack pointer from the stack

    pop eax                                         ; pop the old data segment (to restore the old segments)
    mov ds, ax                                      ; set the data segment to the old value before the exception
    mov es, ax                                      ; same with es
    mov fs, ax                                      ; and fs
    mov gs, ax                                      ; and gs
    popa                                            ; pops all the registers pushed at the start

    add esp, 8                                      ; removes the interrupt number and error code

    iret                                            ; return from interrupt









; based on the exceptions from https://wiki.osdev.org/Exceptions :
; due to all the comparisons this is way slower than just writing all 256 of the ISRs manually
; but that is a lot of lines of code
; this loop creates the ISRs which later gets added to the table
%assign i 0                                         ; assigns the value 0 to i
%rep 256                                            ; repeat 256 times
    %if i == 8                                      ; this big mess of "if"s is equivalent to:
        ISR_WITH_ERR i                              ; interrupts with error:
    %else                                           ;
        %if i >= 10                                 ;
            %if i <= 14                             ; if (i == 8 || (i >= 10 && i <= 14) || i == 17 || i == 21 || i == 29 || i == 30){
                ISR_WITH_ERR i                      ;    ISR_WITH_ERR i
            %else                                   ; } else{ 
                %if i == 17                         ;    ISR_WITHOUT_ERR i
                    ISR_WITH_ERR i                  ; }
                %else                               ;
                    %if i == 21                     ;
                        ISR_WITH_ERR i              ;
                    %else                           ;
                        %if i == 29                 ;
                            ISR_WITH_ERR i          ;
                        %else                       ;
                            %if i == 30             ;
                                ISR_WITH_ERR i      ;
                            %else                   ;
                                ISR_WITHOUT_ERR i   ; interrupt without error
                            %endif                  ;
                        %endif                      ;
                    %endif                          ;
                %endif                              ;
            %endif                                  ;
        %else                                       ; else
            ISR_WITHOUT_ERR i                       ; interrupt without error
        %endif                                      ;
    %endif                                          ;       
    %assign i i+1                                   ; increments i
%endrep




; this loop is the same loop as the one above, but it is to add the ISR codes to the table
global ISR_ERROR_TABLE
ISR_ERROR_TABLE:
%assign i 0 
%rep 256
    %if i == 8
        dd ISR_WITH_ERR_%+i
    %else
        %if i >= 10
            %if i <= 14
                dd ISR_WITH_ERR_%+i
            %else
                %if i == 17
                    dd ISR_WITH_ERR_%+i
                %else
                    %if i == 21
                        dd ISR_WITH_ERR_%+i
                    %else
                        %if i == 29
                            dd ISR_WITH_ERR_%+i
                        %else
                            %if i == 30
                                dd ISR_WITH_ERR_%+i
                            %else
                                dd ISR_WITHOUT_ERR_%+i
                            %endif
                        %endif
                    %endif
                %endif
            %endif
        %else
            dd ISR_WITHOUT_ERR_%+i
        %endif
    %endif
    %assign i i+1 
%endrep








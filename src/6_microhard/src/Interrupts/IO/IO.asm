[bits 32]

; writes to a port
global outputb
outputb:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret


; reads from a port
global inputb
inputb:
    mov dx, [esp + 4]
    in al, dx
    movzx eax, al
    ret

; enables all interrupts again, which is done when the cpu has finished an interrupt
global EnableInterrupts 
EnableInterrupts:
    sti
    ret

; disables all interrupts, which is done when the cpu is already in an interrupt
global DisableInterrupts
DisableInterrupts:
    cli
    ret
[bits 16]

[global _start]
[extern __t_main]

STACK_SEGMENT equ 0x2000

_start:
    cli 
    
    mov ax, cs
    mov ds, ax
    mov es, ax

    mov ax, STACK_SEGMENT
    mov ss, ax
    mov sp, 0xffff
    mov bp, sp
    nop

    xor ax, ax
    mov si, ax
    mov di, ax

    sti

t_entry:
    call __t_main
    jmp $



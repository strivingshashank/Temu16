[bits 16]

[global _start]
[extern __t_init]

MAX_SEGMENT_SIZE equ 0xffff
; STACK_SEGMENT equ 0x17e0
STACK_SEGMENT equ 0x2000

_start:
    cli 
    
    mov ax, cs
    mov ds, ax
    mov es, ax

    mov ax, STACK_SEGMENT
    mov ss, ax
    mov sp, MAX_SEGMENT_SIZE
    mov bp, sp
    nop

    xor ax, ax
    mov si, ax
    mov di, ax

    sti

    call __t_init
    jmp $

    ; times (SECTORS_PER_SEGMENT*BYTES_PER_SECTOR)-($-$$) db 0 ; Pad the rest of the temu sector.



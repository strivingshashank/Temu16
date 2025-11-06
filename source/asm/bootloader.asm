[bits 16]
[org 0x7c00]

TEMU_SECTOR_COUNT equ 128
TEMU_LOAD_SEGMENT equ 0x1000 ; Converts to -> 0x10000 (physical address)
BYTES_PER_SECTOR equ 512

segment_setup:
    mov ax, cs
    mov ds, ax

    mov ax, TEMU_LOAD_SEGMENT
    mov es, ax
    mov bx, 0x0000

disk_setup:
    mov dl, 0 ; Read from drive number.
    mov al, TEMU_SECTOR_COUNT ; Number of sectors to read.
    mov ch, 0 ; Select cylinder number (Base 0; 1st cylinder).       'C'
    mov dh, 0 ; Use the head on the opposite side (Base 0; 1st head).    'H'
    mov cl, 2 ; Select sector number (Base 1; 2nd sector).           'S'
    
    mov ah, 0x02     ; BIOS read disk
    int 0x13

    ; Error handling
    jc .fail
    cmp al, TEMU_SECTOR_COUNT
    je clear_screen

.fail:
    mov al, '!'
    int 0x10
    jmp $ ; infinite loop on failure
    
clear_screen:
    ; Clear screen
    mov cx, (80 * 25)
    mov ah, 0x0e
    mov al, ' '

.clear_loop:
    int 0x10
    dec cx
    cmp cx, 0
    jne .clear_loop

reset_cursor:
    mov bh, 0
    mov dh, 0
    mov dl, 0
    mov ah, 0x02 ; BIOS set cusor
    int 0x10 
        
t_jump:    
    jmp TEMU_LOAD_SEGMENT:0x0000 ; Make the actual jump to the kernel.
    
; Pad the rest of the boot sector.
times (BYTES_PER_SECTOR-2)-($-$$) db 0 
; End with boot sector code.
dw 0xaa55 


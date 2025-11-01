[bits 16]
[org 0x7c00]

TEMU_SECTOR_COUNT equ 128
TEMU_LOAD_SEGMENT equ 0x1000 ; Converts to -> 0x10000 (physical address)
BYTES_PER_SECTOR equ 512

Boot:
    mov ax, cs
    mov ds, ax

    mov ax, TEMU_LOAD_SEGMENT
    mov es, ax
    mov bx, 0x0000

    mov dl, 0 ; Read from drive number.
    mov al, TEMU_SECTOR_COUNT ; Number of sectors to read.
    mov ch, 0x00 ; Select cylinder number (Base 0; 1st cylinder).       'C'
    mov dh, 0x00 ; Use the head on the opposite side (Base 0; 1st head).    'H'
    mov cl, 0x02 ; Select sector number (Base 1; 2nd sector).           'S'
    
    mov ah, 0x02     ; BIOS read disk
    int 0x13

    ; Error handling
    jc .fail
    cmp al, TEMU_SECTOR_COUNT
    jne .fail
    
    jmp TEMU_LOAD_SEGMENT:0x0000 ; Make the actual jump to the kernel.

.fail:
    mov al, '!'
    int 0x10
    jmp $ ; infinite loop on failure
    
times (BYTES_PER_SECTOR-2)-($-$$) db 0 ; Pad the rest of the boot sector.
dw 0xaa55 ; End with boot sector code.


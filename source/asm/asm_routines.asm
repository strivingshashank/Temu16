%ifndef LOW_LEVEL_HELPERS_INCLUDED
%define LOW_LEVEL_HELPERS_INCLUDED 1

[global __cursor_set] ; Needs work
[global __set_display_page]
[global __get_display_page]
[global __screen_scroll]
[global __screen_write_char]
[global __mem_read8]
[global __mem_read16]
[global __mem_write8]
[global __mem_write16]
[global __mem_copy]
[global __kboard_get_key_buffer]
[global __kboard_get_key_blocking]
[global __disk_read]
[global __time_get_hours]
[global __time_get_minutes]
[global __time_get_seconds]
[global __jump_far]

; Required by Bruce's C Compiler
[global idiv_u] ; for '/' (divison) operator
[global imodu]  ; for '%' (modulus) operator

; '/', division operator
idiv_u:
    push dx
    xor dx, dx        ; quotient = 0

.divisionLoop:
    cmp ax, bx
    jb .return
    sub ax, bx
    inc dx
    jmp .divisionLoop

.return:
    mov ax, dx
    pop dx
    ret

; '%', modulo operator
imodu:
    push dx
    xor dx, dx

.modLoop:
    cmp ax, bx        ; if numerator < denominator, done
    jb .return
    sub ax, bx        ; subtract denominator
    jmp .modLoop

.return:
    pop dx
    ret

; NOTE: Macros are only used to make reviewing the code easier, that is all.
STACK_ELEMENT_SIZE equ 2

%macro INTERRUPT_SET_CURSOR 0
    mov ah, 0x02
    int 0x10
%endmacro

%macro INTERRUPT_SET_DISPLAY_PAGE 0
    mov ah, 0x05
    int 0x10
%endmacro

%macro INTERRUPT_GET_DISPLAY_PAGE 0
    mov ah, 0x03
    int 0x10
%endmacro

%macro INTERRUPT_SCREEN_SCROLL_UPWARDS 0
    mov ah, 0x06
    int 0x10
%endmacro

%macro INTERRUPT_WRITE_CHAR 0
    mov ah, 0x0e
    int 0x10
%endmacro

%macro INTERRUPT_GET_KEY 0
    mov ah, 0x00
    int 0x16
%endmacro

%macro INTERRUPT_READ_KBOARD_BUFFER 0
    mov ah, 0x01
    int 0x16
%endmacro

%macro INTERRUPT_DISK_READ 0
    mov ah, 0x02
    int 0x13
%endmacro

%macro INTERRUPT_GET_TIME 0
    mov ah, 0x02
    int 0x1a
%endmacro

; NOTE: This macro assumes that BP is set-up correctly.
; Usage: LOAD_CALLER_ARGUMENT n (1-based)
%macro LOAD_CALLER_ARGUMENT 1
    mov ax, [bp + (%1 * STACK_ELEMENT_SIZE) + STACK_ELEMENT_SIZE] ; The last STACK_ELEMENT_SIZE accounts for the return address.
%endmacro

__cursor_set:
    push bp
    mov bp, sp    
    
    push bx
    push dx

    ; Resolve display page
    LOAD_CALLER_ARGUMENT 1
    mov bh, al
    ; Resolve row index
    LOAD_CALLER_ARGUMENT 2
    mov dh, al
    ; Resolve column index
    LOAD_CALLER_ARGUMENT 3
    mov dl, al

    INTERRUPT_SET_CURSOR

    pop dx
    pop bx
    pop bp
    ret

__get_display_page:
    INTERRUPT_GET_DISPLAY_PAGE
    mov al, bh
    mov ah, 0

    ret

__set_display_page:
    push bp
    mov bp, sp

    mov al, [bp + 4]
    INTERRUPT_SET_DISPLAY_PAGE

    pop bp
    ret

__screen_scroll:
    push cx
    push dx

    mov al, 1 ; Lines to scroll
    mov ch, 0 ; top left line
    mov cl, 0 ; top left column
    mov dh, 24 ; bottom right line
    mov dl, 79 ; bottom right column

    INTERRUPT_SCREEN_SCROLL_UPWARDS

    pop dx
    pop cx
    ret

__screen_write_char:
    push bp
    mov bp, sp

    LOAD_CALLER_ARGUMENT 1
    INTERRUPT_WRITE_CHAR

    pop bp
    ret

__mem_write8:
    push  bp
    mov bp, sp
    
    push es

    LOAD_CALLER_ARGUMENT 1
    mov es, ax
    
    LOAD_CALLER_ARGUMENT 2
    mov di, ax

    LOAD_CALLER_ARGUMENT 3
    mov [es:di], al

    pop es
    pop bp    
    ret

__mem_write16:
    push  bp
    mov bp, sp

    push es

    LOAD_CALLER_ARGUMENT 1
    mov es, ax
    
    LOAD_CALLER_ARGUMENT 2
    mov di, ax

    LOAD_CALLER_ARGUMENT 3
    mov [es:di], ax

    pop es
    pop bp    
    ret

__mem_read8:
    push bp
    mov bp, sp

    push es

    LOAD_CALLER_ARGUMENT 1
    mov es, ax

    LOAD_CALLER_ARGUMENT 2
    mov di, ax
    
    mov ax, [es:di]

    pop es
    pop bp
    ret

__mem_read16:
    push bp
    mov bp, sp

    push es

    LOAD_CALLER_ARGUMENT 1
    mov es, ax    

    LOAD_CALLER_ARGUMENT 2
    mov di, ax
    
    mov ax, [es:di]

    pop es
    pop bp
    ret

__mem_copy:
    push bp
    mov bp, sp

    push ds
    push es
    push si
    push di
    push cx

    LOAD_CALLER_ARGUMENT 1
    mov ds, ax

    LOAD_CALLER_ARGUMENT 2
    mov si, ax

    LOAD_CALLER_ARGUMENT 3
    mov es, ax

    LOAD_CALLER_ARGUMENT 4
    mov di, ax

    LOAD_CALLER_ARGUMENT 5
    mov cx, ax

    cld ; Clear direction flag so that me increment after each iteration
    rep movsb ; Actual copying of data

    pop cx
    pop di
    pop si
    pop es
    pop ds
    pop bp
    ret

__kboard_get_key_buffer:
    INTERRUPT_READ_KBOARD_BUFFER
    jz .noKeyPress

    ; At this point, a key is present in the buffer.
    INTERRUPT_GET_KEY

.return:
    ret

.noKeyPress:
    mov ax, 0
    jmp .return

__kboard_get_key_blocking:
    INTERRUPT_GET_KEY
    ret

__disk_read:
    push bp
    mov bp, sp

    push bx
    push cx
    push dx
    push es

    ; Load from default drive (0)
    mov dl, 0

    ; CHS address
    LOAD_CALLER_ARGUMENT 1
    mov ch, al
    LOAD_CALLER_ARGUMENT 2
    mov dh, al
    LOAD_CALLER_ARGUMENT 3
    mov cl, al

    ; Load destination address
    LOAD_CALLER_ARGUMENT 4
    mov es, ax
    LOAD_CALLER_ARGUMENT 5
    mov bx, ax
    
    ; Sectors to read
    LOAD_CALLER_ARGUMENT 6

    INTERRUPT_DISK_READ
    jc .disk_read_fail

    mov ax, 0

.return:
    pop es
    pop dx
    pop cx
    pop bx
    pop bp
    ret

.disk_read_fail:
    mov al, ah
    mov ah, 0
    jmp .return

__time_get_hours:
    push cx
    push dx

    INTERRUPT_GET_TIME
    mov al, ch

    pop cx
    pop dx
    ret

__time_get_minutes:
    push cx
    push dx

    INTERRUPT_GET_TIME
    mov al, cl

    pop cx
    pop dx
    ret

__time_get_seconds:
    push cx
    push dx

    INTERRUPT_GET_TIME
    mov al, dh

    pop cx
    pop dx
    ret

__jump_far:
    push bp
    mov bp, sp

    ; New code segment
    LOAD_CALLER_ARGUMENT 1
    push ax
    ; New instruction pointer
    LOAD_CALLER_ARGUMENT 2
    push ax

    ; Jump 
    retf

.return:
    pop bp
    ret

%endif


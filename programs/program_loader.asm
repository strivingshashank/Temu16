[bits 16]

[global _start]
[extern __program_init]

_start:
  cli

  mov ax, cs
  mov ds, ax
  mov es, ax

  mov si, 0x00
  mov di, 0x00

  sti

call __program_init

; Jump back to Temu
jmp 0x07e0:0x00

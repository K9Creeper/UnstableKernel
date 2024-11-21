MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
MBFLAGS  equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

section .multiboot
  align 4
  dd MAGIC
  dd MBFLAGS
  dd CHECKSUM
  
section .bss
  system_stack:
    resb 8192 ; 8 KiB
  
  
section .text
  global _start
  
  mov esp, system_stack 

  _start:
    extern kernel_entry
        call kernel_entry

times 510-($ - $$) db 0
dw 0xAA55
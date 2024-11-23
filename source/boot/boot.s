    .section .multiboot
    .align 4
    .long 0x1BADB002              # MAGIC
    .long 0x3                     # MBFLAGS (MBALIGN | MEMINFO)
    .long -(0x1BADB002 + 0x3)     # CHECKSUM

    .section .bss
system_stack:
    .skip 8192                    # 8 KiB for stack

    .section .text
    .global _start
_start:
    movl $system_stack, %esp      # Set stack pointer to system_stack

    # Call kernel_entry
    .extern kernel_entry
    call kernel_entry

    # Padding to make the boot sector 512 bytes
    .org 510
    .byte 0                        # Fill up to byte 510
    .word 0xAA55                   # Boot signature

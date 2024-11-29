.section .bss
stack_bottom:
    .skip 16384          # Reserve space for the stack (16KB)
stack_top:

.section .text
.global _start
.type _start, @function
_start:

    mov $stack_top, %esp

    push %eax
    push %ebx

    call kernel_main

    cli

1:  hlt
    jmp 1b
    
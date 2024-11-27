	  .global _irq0
    .global _irq1
    .global _irq2
    .global _irq3
    .global _irq4
    .global _irq5
    .global _irq6
    .global _irq7
    .global _irq8
    .global _irq9
    .global _irq10
    .global _irq11
    .global _irq12
    .global _irq13
    .global _irq14
    .global _irq15

_irq0:
    cli
    pushb $0
    pushb $32
    jmp irq_common_stub

_irq1:
    cli
    pushb $0
    pushb $33
    jmp irq_common_stub

_irq2:
    cli
    pushb $0
    pushb $34
    jmp irq_common_stub

_irq3:
    cli
    pushb $0
    pushb $35
    jmp irq_common_stub

_irq4:
    cli
    pushb $0
    pushb $36
    jmp irq_common_stub

_irq5:
    cli
    pushb $0
    pushb $37
    jmp irq_common_stub

_irq6:
    cli
    pushb $0
    pushb $38
    jmp irq_common_stub

_irq7:
    cli
    pushb $0
    pushb $39
    jmp irq_common_stub

_irq8:
    cli
    pushb $0
    pushb $40
    jmp irq_common_stub

_irq9:
    cli
    pushb $0
    pushb $41
    jmp irq_common_stub

_irq10:
    cli
    pushb $0
    pushb $42
    jmp irq_common_stub

_irq11:
    cli
    pushb $0
    pushb $43
    jmp irq_common_stub

_irq12:
    cli
    pushb $0
    pushb $44
    jmp irq_common_stub

_irq13:
    cli
    pushb $0
    pushb $45
    jmp irq_common_stub

_irq14:
    cli
    pushb $0
    pushb $46
    jmp irq_common_stub

_irq15:
    cli
    pushb $0
    pushb $47
    jmp irq_common_stub

.extern _irq_handler

irq_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    mov _irq_handler, %eax
    call *%eax
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

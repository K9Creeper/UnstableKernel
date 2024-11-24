    .global _isr0
    .global _isr1
    .global _isr2
    .global _isr3
    .global _isr4
    .global _isr5
    .global _isr6
    .global _isr7
    .global _isr8
    .global _isr9
    .global _isr10
    .global _isr11
    .global _isr12
    .global _isr13
    .global _isr14
    .global _isr15
    .global _isr16
    .global _isr17
    .global _isr18
    .global _isr19
    .global _isr20
    .global _isr21
    .global _isr22
    .global _isr23
    .global _isr24
    .global _isr25
    .global _isr26
    .global _isr27
    .global _isr28
    .global _isr29
    .global _isr30
    .global _isr31

_isr0:
    cli
    pushw   $0            # Correct way to push a word (16-bit immediate value)
    pushw   $0            # Push another 16-bit immediate value (0)
    jmp     isr_common_stub

_isr1:
    cli
    pushw   $0            # Push 0
    pushw   $1            # Push 1
    jmp     isr_common_stub

_isr2:
    cli
    pushw   $0            # Push 0
    pushw   $2            # Push 2
    jmp     isr_common_stub

_isr3:
    cli
    pushw   $0            # Push 0
    pushw   $3            # Push 3
    jmp     isr_common_stub

_isr4:
    cli
    pushw   $0            # Push 0
    pushw   $4            # Push 4
    jmp     isr_common_stub

_isr5:
    cli
    pushw   $0            # Push 0
    pushw   $5            # Push 5
    jmp     isr_common_stub

_isr6:
    cli
    pushw   $0            # Push 0
    pushw   $6            # Push 6
    jmp     isr_common_stub

_isr7:
    cli
    pushw   $0            # Push 0
    pushw   $7            # Push 7
    jmp     isr_common_stub

_isr8:
    cli
    pushw   $8            # Push 8
    jmp     isr_common_stub

_isr9:
    cli
    pushw   $0            # Push 0
    pushw   $9            # Push 9
    jmp     isr_common_stub

_isr10:
    cli
    pushw   $10           # Push 10
    jmp     isr_common_stub

_isr11:
    cli
    pushw   $11           # Push 11
    jmp     isr_common_stub

_isr12:
    cli
    pushw   $12           # Push 12
    jmp     isr_common_stub

_isr13:
    cli
    pushw   $13           # Push 13
    jmp     isr_common_stub

_isr14:
    cli
    pushw   $14           # Push 14
    jmp     isr_common_stub

_isr15:
    cli
    pushw   $0            # Push 0
    pushw   $14           # Push 14
    jmp     isr_common_stub

_isr16:
    cli
    pushw   $0            # Push 0
    pushw   $16           # Push 16
    jmp     isr_common_stub

_isr17:
    cli
    pushw   $17           # Push 17
    jmp     isr_common_stub

_isr18:
    cli
    pushw   $0            # Push 0
    pushw   $18           # Push 18
    jmp     isr_common_stub

_isr19:
    cli
    pushw   $0            # Push 0
    pushw   $19           # Push 19
    jmp     isr_common_stub

_isr20:
    cli
    pushw   $0            # Push 0
    pushw   $20           # Push 20
    jmp     isr_common_stub

_isr21:
    cli
    pushw   $21           # Push 21
    jmp     isr_common_stub

_isr22:
    cli
    pushw   $0            # Push 0
    pushw   $22           # Push 22
    jmp     isr_common_stub

_isr23:
    cli
    pushw   $0            # Push 0
    pushw   $23           # Push 23
    jmp     isr_common_stub

_isr24:
    cli
    pushw   $0            # Push 0
    pushw   $24           # Push 24
    jmp     isr_common_stub

_isr25:
    cli
    pushw   $0            # Push 0
    pushw   $25           # Push 25
    jmp     isr_common_stub

_isr26:
    cli
    pushw   $0            # Push 0
    pushw   $26           # Push 26
    jmp     isr_common_stub

_isr27:
    cli
    pushw   $0            # Push 0
    pushw   $27           # Push 27
    jmp     isr_common_stub

_isr28:
    cli
    pushw   $0            # Push 0
    pushw   $28           # Push 28
    jmp     isr_common_stub

_isr29:
    cli
    pushw   $29           # Push 29
    jmp     isr_common_stub

_isr30:
    cli
    pushw   $30           # Push 30
    jmp     isr_common_stub

_isr31:
    cli
    pushw   $0            # Push 0
    pushw   $31           # Push 31
    jmp     isr_common_stub

.extern _fault_handler

isr_common_stub:
    pusha
    pushl   %ds
    pushl   %es
    pushl   %fs
    pushl   %gs
    movw    $0x10, %ax        # Load the Kernel Data Segment descriptor
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs
    movl    %esp, %eax        # Push the stack pointer
    pushl   %eax
    movl    $_fault_handler, %eax
    call    *%eax              # Call the fault handler (indirect call)
    popl    %eax
    popl    %gs
    popl    %fs
    popl    %es
    popl    %ds
    popa
    addl    $8, %esp          # Cleans up the pushed error code and ISR number
    iret

.extern _irq_handler

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
    cli                     # Clear interrupt flag
    pushw   $0              # Push byte 0
    pushw   $32             # Push byte 32 (IRQ number)
    jmp     irq_common_stub

_irq1:
    cli
    pushw   $0
    pushw   $33
    jmp     irq_common_stub

_irq2:
    cli
    pushw   $0
    pushw   $34
    jmp     irq_common_stub

_irq3:
    cli
    pushw   $0
    pushw   $35
    jmp     irq_common_stub

_irq4:
    cli
    pushw   $0
    pushw   $36
    jmp     irq_common_stub

_irq5:
    cli
    pushw   $0
    pushw   $37
    jmp     irq_common_stub

_irq6:
    cli
    pushw   $0
    pushw   $38
    jmp     irq_common_stub

_irq7:
    cli
    pushw   $0
    pushw   $39
    jmp     irq_common_stub

_irq8:
    cli
    pushw   $0
    pushw   $40
    jmp     irq_common_stub

_irq9:
    cli
    pushw   $0
    pushw   $41
    jmp     irq_common_stub

_irq10:
    cli
    pushw   $0
    pushw   $42
    jmp     irq_common_stub

_irq11:
    cli
    pushw   $0
    pushw   $43
    jmp     irq_common_stub

_irq12:
    cli
    pushw   $0
    pushw   $44
    jmp     irq_common_stub

_irq13:
    cli
    pushw   $0
    pushw   $45
    jmp     irq_common_stub

_irq14:
    cli
    pushw   $0
    pushw   $46
    jmp     irq_common_stub

_irq15:
    cli
    pushw   $0
    pushw   $47
    jmp     irq_common_stub

irq_common_stub:
    pusha                   # Save all general-purpose registers
    pushl   %ds             # Push DS
    pushl   %es             # Push ES
    pushl   %fs             # Push FS
    pushl   %gs             # Push GS
    movw    $0x10, %ax      # Set segment descriptor (0x10 is Kernel DS)
    movw    %ax, %ds        # Set DS
    movw    %ax, %es        # Set ES
    movw    %ax, %fs        # Set FS
    movw    %ax, %gs        # Set GS
    movl    %esp, %eax      # Store ESP in EAX
    pushl   %eax            # Push ESP for IRQ handler
    movl    $_irq_handler, %eax  # Load address of IRQ handler
    call    *%eax           # Call IRQ handler
    popl    %eax            # Restore EAX
    popl    %gs             # Restore GS
    popl    %fs             # Restore FS
    popl    %es             # Restore ES
    popl    %ds             # Restore DS
    popa                    # Restore all general-purpose registers
    addl    $8, %esp        # Clean up stack (remove IRQ handler params)
    iret                    # Return from interrupt

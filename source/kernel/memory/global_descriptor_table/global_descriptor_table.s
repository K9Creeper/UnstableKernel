    .global _gdt_flush
    .extern _pGDT

_gdt_flush:
    lgdt [ _pGDT ]          # Load Global Descriptor Table (lgdt)
    mov    $0x10, %ax
    mov    %ax, %ds
    mov    %ax, %es
    mov    %ax, %fs
    mov    %ax, %gs
    mov    %ax, %ss

    jmp    flush2

flush2:
    ret
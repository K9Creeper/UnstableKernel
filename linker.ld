ENTRY(_start)

SECTIONS
{
    /* Start address of the program */
    /*. = 0xC0100000;   */
    . = 0x100000;
    linkerld_startofkernel = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    linkerld_endofkernel = .;
}
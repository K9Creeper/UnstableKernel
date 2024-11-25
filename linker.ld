ENTRY(_start)

SECTIONS
{
    . = 2M;  /* Start address of the program */

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

    linkerld_endofprogram = .;  /* Set 'end' symbol to the current address (end of the last section) */
}
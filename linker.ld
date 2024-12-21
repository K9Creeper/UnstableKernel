ENTRY(_start)

phys = 0x100000;
offset = 0xC0000000;
virt = offset + phys;

SECTIONS
{
    . = virt;
    linkerld_startofkernel = .;
    .text : AT(ADDR(.text)-0xC0000000)
    {
        *(.multiboot)
        *(.text)
    }
    .rodata ALIGN (4K) : AT(ADDR(.rodata)-0xC0000000)
    {
        *(.rodata)
    }
     
    .bss ALIGN(4K) : AT(ADDR(.bss)-0xC0000000)
    {
        *(.COMMON)
        *(.bss)
    }

    .data ALIGN(4K) : AT(ADDR(.data)-0xC0000000)
    {
        *(.data)
    }
    
    linkerld_endofkernel = .;
}
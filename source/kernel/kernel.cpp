#include "../chelpers/multiboot.h"

#include "memory/memory.hpp"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"

#include "multiboot/multiboot.hpp"

#include "input/keyboard.hpp"

#include "debug/serial.hpp"

#include "graphics/graphics.hpp"

// May be a good source to look at: https://github.com/collinsmichael/spartan/

extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    asm volatile("cli");

    Kernel::Multiboot::mb_info = addr;
    Kernel::Multiboot::mb_magic = magic;

    Kernel::Debug::COM1::Init();
    printf("In kernel_main!\n");

    Kernel::Memory::InitMemInfo();

    printf("Basic mem info, lower: 0x%X | upper: 0x%X\n", Kernel::Memory::Info::mem_lower, Kernel::Memory::Info::mem_upper);

    multiboot_tag_framebuffer_common *fbc = Kernel::Multiboot::GetFrameBuffer();
    printf("Framebuffer located at 0x%X\n", fbc->framebuffer_addr);

    Kernel::Memory::GDT::Init();
    printf("Initialized GDT\n");

    Kernel::Memory::GDT::Install();
    printf("Installed GDT\n");

    Kernel::Memory::IDT::Init();
    printf("Initialized IDT\n");

    Kernel::Memory::IDT::Install();
    printf("Installed IDT\n");

    for (;;)
        asm volatile("hlt");
}
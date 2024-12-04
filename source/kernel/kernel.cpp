#include "../chelpers/multiboot.h"

#include "memory/memory.hpp"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"

#include "memory/paging/paging.hpp"
#include "memory/kheap/kheap.hpp"

#include "multiboot/multiboot.hpp"

#include "debug/serial.hpp"

// May be a good source to look at: https://github.com/collinsmichael/spartan/ and https://github.com/szhou42/osdev/tree/master

extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    asm volatile("cli");

    Kernel::Multiboot::mb_info = addr;
    Kernel::Multiboot::mb_magic = magic;

    Kernel::Debug::COM1::Init();
    printf("In kernel_main!\n");

    Kernel::Memory::InitMemInfo();

    printf("Basic Multiboot Memory Info | Lower: 0x%X | Upper: 0x%X\n", Kernel::Memory::Info::mem_lower, Kernel::Memory::Info::mem_upper);
    printf("Kernel Memory Info | Start: 0x%X | end: 0x%X\n", Kernel::Memory::Info::kernel_start, Kernel::Memory::Info::kernel_end);

    Kernel::Memory::KHeap::Early::PreInit(Kernel::Memory::Info::kernel_end);

    Kernel::Memory::GDT::Init();
    printf("Initialized GDT\n");

    Kernel::Memory::GDT::Install();
    printf("Installed GDT\n");

    Kernel::Memory::IDT::Init();
    printf("Initialized IDT\n");

    Kernel::Memory::IDT::Install();
    printf("Installed IDT\n");

    Kernel::Memory::Paging::Init();

    for (;;)
        asm volatile("hlt");
}
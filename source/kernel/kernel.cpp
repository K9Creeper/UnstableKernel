#include "../chelpers/multiboot.h"

#include "memory/memory.hpp"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"
#include "memory/task_state_segment/task_state_segment.hpp"

#include "memory_management/physical_memory_manager/physical_memory_manager.hpp"
#include "memory_management/paging/paging.hpp"
#include "memory_management/kheap/kheap.hpp"

#include "bios32/bios32.hpp"

#include "multiboot/multiboot.hpp"

#include "drivers/debug/serial.hpp"
#include "drivers/timer/timer.hpp"

#include "drivers/vesa/vesa.hpp"

// May be a good source to look at: https://github.com/collinsmichael/spartan/ and https://github.com/szhou42/osdev/tree/master

void draw_test_pattern(uint32_t lfb_base, uint32_t width, uint32_t pitch, uint32_t bpp)
{
    for (uint32_t i = 0; i < 500; i++)
    {
        for (uint32_t k = 0; k < 500; k++)
        {
            uint32_t pixel_offset = i * pitch + (k * (bpp / 8)) + lfb_base;

            uint32_t *loc = reinterpret_cast<uint32_t *>(pixel_offset);

            *loc = 0xFFFFFFFF;
        }
    }
}

extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    asm volatile("cli");

    Kernel::Multiboot::mb_info = addr;
    Kernel::Multiboot::mb_magic = magic;

    Kernel::Debug::COM1::Init();

    printf("In C Kernel\n");
    Kernel::Memory::InitMemInfo();

    printf("Kernel Memory Info | Start: 0x%X | end: 0x%X\n", Kernel::Memory::Info::kernel_start, Kernel::Memory::Info::kernel_end);

    Kernel::MemoryManagement::KHeap::Early::PreInit(Kernel::Memory::Info::kernel_end);

    Kernel::Memory::GDT::Init();
    printf("Initialized | GDT\n");

    Kernel::Memory::GDT::Install();
    printf("Installed | GDT\n");

    Kernel::Memory::TSS::Init(5, 0x10, 0);
    printf("Installed | TSS\n");

    Kernel::Memory::IDT::Init();
    printf("Initialized | IDT\n");

    Kernel::Memory::IDT::Install();
    printf("Installed | Interrupts & IDT\n");

    Kernel::Bios32::Init();
    printf("Initialized | Bios32 Service\n");

    Kernel::MemoryManagement::PMM::Init(Kernel::Memory::Info::pmm_size);
    printf("Initialzied | PMM\n");

    Kernel::MemoryManagement::Paging::Init();
    printf("Initialzied | Paging\n");

    Kernel::Drivers::VESA::Init(1024, 768, 32);
    printf("Initialized | VESA\n");

    Kernel::MemoryManagement::KHeap::Init(0xC0400000, 0xC0400000 + 0x100000, 0xCFFFF000);
    printf("Initialized & Installed | KHeap\n");

    // printf("LFB Address 0x%X\n", Kernel::Drivers::VESA::GetLFBAddress());

    draw_test_pattern(Kernel::Drivers::VESA::GetLFBAddress(), Kernel::Drivers::VESA::currentMode.info.width, Kernel::Drivers::VESA::currentMode.info.pitch, 32);

    for (;;)
        asm volatile("hlt");
}
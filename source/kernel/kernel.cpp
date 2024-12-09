#include "../chelpers/multiboot.h"

#include "memory/memory.hpp"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"
#include "memory/task_state_segment/task_state_segment.hpp"

#include "memory_management/paging/paging.hpp"
#include "memory_management/kheap/kheap.hpp"

#include "bios32/bios32.hpp"

#include "multiboot/multiboot.hpp"

#include "drivers/debug/serial.hpp"
#include "drivers/timer/timer.hpp"

#include "drivers/vesa/vesa.hpp"

// May be a good source to look at: https://github.com/collinsmichael/spartan/ and https://github.com/szhou42/osdev/tree/master

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

    Kernel::MemoryManagement::Paging::Init(Kernel::Memory::Info::pmm_size);
    printf("Initialized & Installed | PMM (size of 0x%X) & Paging\n", Kernel::Memory::Info::pmm_size);
    
    Kernel::MemoryManagement::KHeap::Init(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX_END, 0, 0);
    printf("Initialized & Installed | KHeap\n");

    Kernel::Bios32::Init();
    printf("Initialized | Bios32 Service\n");

    Kernel::Drivers::Timer::Init(100);
    printf("Initialized & Installed | Timer\n");

    Kernel::Drivers::VESA::Init();
    printf("Initialized | VESA\n");


    for (;;)
        asm volatile("hlt");
}
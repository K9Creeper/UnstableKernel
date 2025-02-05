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
#include "drivers/input/keyboard.hpp"

#include "drivers/vesa/vesa.hpp"

#include "../tasking/scheduler/scheduler.hpp"
#include "../tasking/process/process.hpp"

// May be a good source to look at: https://github.com/collinsmichael/spartan/ and https://github.com/szhou42/osdev/tree/master

void KeyboardHandler(const Kernel::Drivers::Input::Keyboard::Key &k, const Kernel::Drivers::Input::Keyboard::Key *keymap)
{
    // uint32_t* pixel = reinterpret_cast<uint32_t*>(Kernel::Drivers::VESA::GetLFBAddress() + (y * Kernel::Drivers::VESA::currentMode.info.pitch + (x * (Kernel::Drivers::VESA::currentMode.info.bpp / 8))));
    //(*pixel)++;
}

/* void Test(){
    printf("What did i say bro?\n");
    
    for(;;);
} */

extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    asm volatile("cli");

    Kernel::Multiboot::mb_info = addr;
    Kernel::Multiboot::mb_magic = magic;

    Kernel::Debug::COM1::Init();

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

    Kernel::Drivers::VESA::Init(1024, 768);
    printf("Initialized | VESA\n");
    printf("Width: %D, Height: %D, Bytes Per Pixel: %D\n", Kernel::Drivers::VESA::currentMode.info.width, Kernel::Drivers::VESA::currentMode.info.height, Kernel::Drivers::VESA::currentMode.info.bpp);

    Kernel::MemoryManagement::KHeap::Init(0xC0400000, 0xC0400000 + 0x100000, 0xCFFFF000);
    printf("Initialized & Installed | KHeap\n");
    printf("Located at 0x%X, Initial End at 0x%X, Max Address 0x%X\n", Kernel::Memory::Info::kheap_start, Kernel::Memory::Info::kheap_end, Kernel::Memory::Info::kheap_max_address);

    Kernel::Drivers::Input::Keyboard::Init();
    printf("Initialized | Keyboard\n");

    Kernel::Drivers::Input::Keyboard::AddHandle(KeyboardHandler);
    printf("Initialized | Keyboard Handle\n");

    /*
    Kernel::Drivers::Timer::Init(100);
    printf("Initialized | Timer\n");

    Tasking::Scheduler::Init();
    printf("Initialized | Tasking Scheduler\n");

    Tasking::Scheduler::Start();
    printf("Starting | Tasking Scheduler\n");

    Tasking::CreateProccessFromFunction(Test, "Test");
    printf("Created a Proccess!\n");
    */

    asm volatile("sti");
    
    for (;;)
        asm volatile("hlt");
}
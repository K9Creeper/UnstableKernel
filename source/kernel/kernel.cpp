#include "../chelpers/multiboot.h"

#include "../chelpers/memory.h"

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
#include "drivers/input/keyboard.hpp"
#include "drivers/input/mouse.hpp"

#include "drivers/pit/pit.hpp"

#include "drivers/vesa/vesa.hpp"

#include "drivers/pci/pci.hpp"

#include "../graphics/graphics.hpp"

#include "multitasking/multitasking.hpp"

// May be a good source to look at: https://github.com/collinsmichael/spartan/ and https://github.com/szhou42/osdev/tree/master

extern "C" void GraphicsThread();

extern void KeyboardHandler(const KeyboardKey &k, const KeyboardKey *keymap);
extern void MouseHandler(const MouseInfo &info);

void SetupEarlyMemory(const uint32_t &addr, const uint32_t &magic)
{
    printf("\n| Setup Early Memory |\n\n");

    Kernel::Multiboot::mb_info = addr;
    Kernel::Multiboot::mb_magic = magic;

    Kernel::Memory::InitMemInfo();
    printf("Kernel Memory Info | Start: 0x%X | end: 0x%X\n", Kernel::Memory::Info::kernel_start, Kernel::Memory::Info::kernel_end);

    Kernel::MemoryManagement::KHeap::Early::PreInit(Kernel::Memory::Info::kernel_end);

    Kernel::Memory::GDT::Init();
    printf("Initialized | GDT\n");

    Kernel::Memory::GDT::Install();
    printf("Installed | GDT\n");

    uint32_t esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    Kernel::Memory::TSS::Init(5, 0x10, esp);
    printf("Installed | TSS\n");

    Kernel::Memory::IDT::Init();
    printf("Initialized | IDT\n");

    Kernel::Memory::IDT::Install();
    printf("Installed | Interrupts & IDT\n");

    Kernel::Bios32::Init();
    printf("Initialized | Bios32 Service\n");

    printf("\n| ------------------ |\n\n");
}

void SetupMemoryManagement()
{
    printf("\n| Setup Memory Management |\n\n");

    Kernel::MemoryManagement::PMM::Init(Kernel::Memory::Info::pmm_size);
    printf("Initialzied | PMM\n");

    Kernel::MemoryManagement::Paging::Init();
    printf("Initialzied | Paging\n");

    Kernel::MemoryManagement::KHeap::Init(0xC0400000, 0xC0500000, 0xCFFFFF00);
    printf("Initialized & Installed | KHeap\n");
    printf("Located at 0x%X, Initial End at 0x%X, Max Address 0x%X\n",
           Kernel::Memory::Info::kheap_start,
           Kernel::Memory::Info::kheap_end,
           Kernel::Memory::Info::kheap_max_address);

    printf("\n| ----------------------- |\n\n");
}

void SetupDrivers()
{
    printf("\n| Setup Drivers |\n\n");

    Kernel::Drivers::PIT::Init();
    printf("Initialized | PIT\n");

    Kernel::Drivers::Input::Keyboard::Init();
    printf("Initialized | Keyboard\n");

    Kernel::Drivers::Input::Mouse::Init();
    printf("Initialized | Mouse\n");

    Kernel::Drivers::PCI::Init();
    printf("Initialized | PCI\n");

    printf("\n| ------------- |\n\n");
}

void SetupGraphics()
{
    printf("\n| Setup Graphics |\n\n");

    Kernel::Drivers::VESA::Init(640, 480);
    printf("Initialized | VESA\n");
    printf("Width: %D, Height: %D, Bytes Per Pixel: %D\n",
           Kernel::Drivers::VESA::currentMode.info.width,
           Kernel::Drivers::VESA::currentMode.info.height,
           Kernel::Drivers::VESA::currentMode.info.bpp);

    Graphics::Init(Kernel::Drivers::VESA::GetLFBAddress(), Kernel::Drivers::VESA::currentMode.info.width, Kernel::Drivers::VESA::currentMode.info.height, Kernel::Drivers::VESA::currentMode.info.pitch, Kernel::Drivers::VESA::currentMode.info.bpp);
    printf("Initialized | Graphics\n");

    Kernel::Multitasking::CreateTask("GraphicsThread", GraphicsThread);

    printf("\n| -------------- |\n\n");
}

void SetupMultitasking()
{
    printf("\n| Setup Multitasking |\n\n");

    Kernel::Multitasking::Init();
    Kernel::Multitasking::Run();

    printf("\n| ------------------ |\n\n");
}

extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    asm volatile("cli");

    Kernel::Debug::COM1::Init();
    printf("-- This is the Kernel Debug Log --\n\n");

    // Set up Tables, KHeap Info, and General Memory Info
    SetupEarlyMemory(addr, magic);

    // Set up PMM, Paging, KHeap
    SetupMemoryManagement();

    SetupDrivers();

    SetupMultitasking();

    SetupGraphics();

    asm volatile("sti");

    for (;;)
    {
        asm volatile("hlt");
    }
}
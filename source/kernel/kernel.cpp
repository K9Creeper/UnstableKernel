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

#define cli asm volatile("cli"); printf("\n| Disabled Interupts |\n\n"); 
#define sti asm volatile("sti"); printf("\n| Enabled Interupts |\n\n");

void SetupEarlyMemory(const uint32_t &addr, const uint32_t &magic)
{
    printf("\n| Setup Early Memory |\n\n");

    Kernel::Multiboot::mb_info = addr;
    Kernel::Multiboot::mb_magic = magic;

    Kernel::Memory::InitMemInfo();
    printf("Kernel Memory Info | Start: 0x%X | End: 0x%X\n", Kernel::Memory::Info::kernel_start, Kernel::Memory::Info::kernel_end);

    Kernel::MemoryManagement::kheap.PreInit(Kernel::Memory::Info::kernel_end);

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
    printf("Initialzied | PMM | Size: 0x%X -- %D MB\n", Kernel::Memory::Info::pmm_size, (Kernel::Memory::Info::pmm_size / 0x100000));

    Kernel::MemoryManagement::InitPaging();
    printf("Initialzied | Paging\n");

    Kernel::MemoryManagement::kheap.Init(0xC0400000, 0xC0500000, 0xCFFFFF00, false, false, &Kernel::MemoryManagement::pManager);
    printf("Initialized & Installed | KHeap\n");
    printf("Located at 0x%X, Initial End at 0x%X, Max Address 0x%X\n",
        Kernel::MemoryManagement::kheap.GetHeapStart(),
        Kernel::MemoryManagement::kheap.GetHeapEnd(),
        Kernel::MemoryManagement::kheap.GetHeapMax());

    printf("\n| ----------------------- |\n\n");
}

void SetupDrivers()
{
    printf("\n| Setup Drivers |\n\n");

    Kernel::Drivers::PIT::Init(100);
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

    printf("\n| -------------- |\n\n");
}

void SetupMultitasking()
{
    printf("\n| Setup Multitasking |\n\n");

    Kernel::Multitasking::Init();
    Kernel::Multitasking::Run();

    printf("\n| ------------------ |\n\n");
}

extern "C" void UsermodeEntry();
extern "C" void UsermodeSyscallTest();

void EnterUsermode(){
    printf("\n| Entering Usermode |\n\n");

    Kernel::Multitasking::CreateTask("UsermodeEntry", UsermodeEntry);
    //Kernel::Multitasking::CreateTask("UsermodeSyscallTest", UsermodeSyscallTest);
}

extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    cli

    Kernel::Debug::COM1::Init();
    printf("-- This is the Kernel Debug Log --\n\n");

    // Set up Tables, KHeap Info, and General Memory Info
    SetupEarlyMemory(addr, magic);

    // Set up PMM, Paging, KHeap
    SetupMemoryManagement();

    SetupDrivers();

    SetupMultitasking();

    sti

    SetupGraphics();

    EnterUsermode();

    for (;;)
    {
        asm volatile("hlt");
    }
}
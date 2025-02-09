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

#include "drivers/vesa/vesa.hpp"

#include "drivers/pci/pci.hpp"

#include "../graphics/framebuffer.hpp"

// May be a good source to look at: https://github.com/collinsmichael/spartan/ and https://github.com/szhou42/osdev/tree/master

void KeyboardHandler(const Kernel::Drivers::Input::Keyboard::Key &k, const Kernel::Drivers::Input::Keyboard::Key *keymap)
{

}

Framebuffer f1;
Framebuffer f2;

extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    asm volatile("cli");

    Kernel::Multiboot::mb_info = addr;
    Kernel::Multiboot::mb_magic = magic;

    Kernel::Debug::COM1::Init();
    printf("-- This is the Kernel Debug Log --\n\n");

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

    Kernel::Drivers::VESA::Init(640, 480);
    printf("Initialized | VESA\n");
    printf("Width: %D, Height: %D, Bytes Per Pixel: %D\n", Kernel::Drivers::VESA::currentMode.info.width, Kernel::Drivers::VESA::currentMode.info.height, Kernel::Drivers::VESA::currentMode.info.bpp);

    Kernel::MemoryManagement::KHeap::Init(0xC0400000, 0xC0400000 + (1 * 0x100000), 0xCFFFF000);
    printf("Initialized & Installed | KHeap\n");
    printf("Located at 0x%X, Initial End at 0x%X, Max Address 0x%X\n", Kernel::Memory::Info::kheap_start, Kernel::Memory::Info::kheap_end, Kernel::Memory::Info::kheap_max_address);

    printf("Gonna malloc...\n");
    char* c = reinterpret_cast<char*>(Kernel::MemoryManagement::KHeap::kmalloc_(3));
    c[0] = 'b';
    c[1] = 'c';
    c[3] = '\0';
    printf("Did malloc %s...\n", c);

    f1.Set(reinterpret_cast<uint32_t*>(Kernel::Drivers::VESA::GetLFBAddress()), Kernel::Drivers::VESA::currentMode.info.width, Kernel::Drivers::VESA::currentMode.info.height, Kernel::Drivers::VESA::currentMode.info.pitch, Kernel::Drivers::VESA::currentMode.info.bpp);

    printf("Gonna malloc... a size of 0x%X\n", f1.GetSize());

    uint32_t* data = reinterpret_cast<uint32_t*>(Kernel::MemoryManagement::KHeap::kmalloc_(f1.GetSize()));

    printf("Did a malloc... at 0x%X\n", data);

    f2.Set(data, Kernel::Drivers::VESA::currentMode.info.width, Kernel::Drivers::VESA::currentMode.info.height, Kernel::Drivers::VESA::currentMode.info.pitch, Kernel::Drivers::VESA::currentMode.info.bpp);
    

    Kernel::Drivers::Input::Keyboard::Init();
    printf("Initialized | Keyboard\n");
    
    Kernel::Drivers::Input::Mouse::Init();
    printf("Initialized | Mouse\n");

    Kernel::Drivers::Input::Keyboard::AddHandle(KeyboardHandler);
    printf("Initialized | Keyboard Handle\n");

    Kernel::Drivers::PCI::Init();
    printf("Initialized | PCI\n");

    asm volatile("sti");

    for (;;){
        f1.Clear();

        int mX = Kernel::Drivers::Input::Mouse::MouseInfo::X;
        int mY = Kernel::Drivers::Input::Mouse::MouseInfo::Y;

        for(int x = mX; x < mX + 15; x++)
        {
            for(int y = mY; y < mY + 15; y++)
            {
                f1.PutPixel(x, y, 0xFFFFFF);
            }
        }

        //f1.Swap(&f2);

        asm volatile("hlt");
    }
}
#include "../c_helpers/multiboot.h"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"

#include "memory/kheap/kheap.hpp"
#include "memory/paging/paging.hpp"

#include "terminal/terminal.hpp"

#include "multiboot/multiboot.hpp"

#include "input/keyboard.hpp"

extern "C" void jump_usermode(void);

extern uint32_t linkerld_startofkernel;

extern "C" void printf(const char *format, ...);

extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    Kernel::Terminal::Init();
    
    Kernel::Memory::GDT::Init();
    Kernel::Memory::GDT::Install();

    Kernel::Memory::IDT::Init();

    Kernel::Memory::IDT::Install();

    Kernel::Memory::Paging::Init(KHEAP_START);

    Kernel::Memory::KHeap::Init(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, false, false);

    Kernel::Input::Keyboard::Init();

    // Enable interrupts
    asm volatile("sti");

    multiboot_tag_framebuffer_common *fbc = Kernel::Multiboot::GetFrameBuffer(addr);
    

    for (;;)
        asm volatile("hlt");
}
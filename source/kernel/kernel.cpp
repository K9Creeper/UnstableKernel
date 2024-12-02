#include "../c_helpers/multiboot.h"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"

#include "memory/kheap/kheap.hpp"
#include "memory/paging/paging.hpp"

#include "terminal/terminal.hpp"

#include "multiboot.hpp"

#include "input/keyboard.hpp"

extern "C" void jump_usermode(void);

extern uint32_t linkerld_startofkernel;

extern "C" void printf(const char *format, ...);

void test(const Kernel::Input::Keyboard::Key& k)
{
	printf("Pressed %s %d %d->%d\n", k.keyname, (int)k.value, k.bPressedPrev, k.bPressed);
}

extern "C" void kernel_main(void)
{
    Kernel::Terminal::Init();
    printf("Terminal Init\n");

    Kernel::Memory::GDT::Init();
    Kernel::Memory::GDT::Install();
    printf("GDT Init\n");

    Kernel::Memory::IDT::Init();
    printf("IDT Init\n");

    Kernel::Memory::IDT::Install();
	printf("Interrupts Init\n");

    Kernel::Memory::Paging::Init(0xC0000000);
    Kernel::Memory::KHeap::Init(0xC0000000, 0xC0000000+KHEAP_INITIAL_SIZE, 0xCFFFF000, false, false);

    Kernel::Input::Keyboard::Init();

    // Enable interrupts
    asm volatile("sti");

    Kernel::Input::Keyboard::AddHandle((void*)test);

    for (;;)
        asm volatile("hlt");
}
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
    if(!k.bPressedPrev && k.bPressedPrev != k.bPressed && k.bPressed)
	    printf("Pressed %s\n", k.keyname);
    else if(k.bPressedPrev && k.bPressedPrev != k.bPressed && !k.bPressed)
        printf("Released %s\n", k.keyname);
    else if(k.bPressedPrev && k.bPressed == k.bPressedPrev)
        printf("Holding %s\n", k.keyname);
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

    Kernel::Memory::Paging::Init(KHEAP_START);
    printf("Paging Enabled\n");
    
    Kernel::Memory::KHeap::Init(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, false, false);
    printf("KHeap Init\n");

    Kernel::Terminal::Clear();

    Kernel::Input::Keyboard::Init();

    // Enable interrupts
    asm volatile("sti");

    Kernel::Input::Keyboard::AddHandle((void*)test);

    for (;;)
        asm volatile("hlt");
}
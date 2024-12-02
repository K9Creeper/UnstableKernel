#include "../c_helpers/multiboot.h"

#include "memory/global_descriptor_table/global_descriptor_table.h"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.h"

#include "memory/interrupt_service_routines/interrupt_service_routines.h"
#include "memory/interrupt_request/interrupt_request.h"

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
	printf("Test\n");
}

extern "C" void kernel_main(void)
{
    Kernel::Terminal::Init();
    printf("Terminal Init\n");

    Kernel_Memory_GDT_Init();
    Kernel_Memory_GDT_Install();
    printf("GDT Init\n");

    Kernel_Memory_IDT_Init();
    printf("IDT Init\n");

	Kernel_Memory_ISR_Install();
    printf("ISRS Init\n");
    Kernel_Memory_IRQ_Install();
    printf("IRQ Init\n");

    // Enable interrupts
    //asm volatile("sti");

    for (;;)
        asm volatile("hlt");
}
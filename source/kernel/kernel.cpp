#include "../c_helpers/multiboot.h"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"
#include "memory/task_state_segment/task_state_segment.hpp"

#include "memory/interrupt_service_routines/interrupt_service_routines.hpp"
#include "memory/interrupt_request/interrupt_request.hpp"

#include "memory/kheap/kheap.hpp"
#include "memory/paging/paging.hpp"

#include "terminal/terminal.hpp"

#include "multiboot.hpp"

#include "input/keyboard.hpp"

extern "C" void jump_usermode(void);

extern uint32_t linkerld_startofkernel;

extern void printf(const char *format, ...);

extern "C" void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info_address)
{
	Kernel::Terminal::Init();

	Kernel::Memory::GDT::Init();
	Kernel::Memory::GDT::Install();

	Kernel::Memory::TSS::Install();

	Kernel::Memory::IDT::Init();
	Kernel::Memory::IDT::Install();

	Kernel::Memory::IRQ::Install();

	Kernel::Memory::ISR::Install();
	
	Kernel::Memory::Paging::Init();

	Kernel::Input::Keyboard::Init();

	// This is wrong
	//Kernel::Multiboot::ParsedMultibootInfo multiboot_info = Kernel::Multiboot::ParseTags(multiboot_info_address);



	for (;;)
		asm volatile("hlt");
}
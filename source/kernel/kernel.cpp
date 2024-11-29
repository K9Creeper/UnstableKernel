#include "../c_helpers/multiboot.h"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"
#include "memory/task_state_segment/task_state_segment.hpp"

#include "memory/interrupt_service_routines/interrupt_service_routines.hpp"
#include "memory/interrupt_request/interrupt_request.hpp"

#include "memory/kheap/kheap.hpp"
#include "memory/paging/paging.hpp"

#include "terminal/terminal.hpp"

#include "../c_helpers/string.h"

#include "multiboot.hpp"

extern "C" void jump_usermode(void);

extern uint32_t linkerld_startofkernel;

extern "C" void kernel_main(uint32_t multiboot_info_address, uint32_t multiboot_magic)
{
	Kernel::Terminal::Init();
	
	char buffer[32];
	itoa(multiboot_magic, buffer, 16);
	Kernel::Terminal::WriteString("Magic: ");
	Kernel::Terminal::WriteString(buffer);
	Kernel::Terminal::WriteString("\n");

	Kernel::Memory::GDT::Init();
	Kernel::Memory::GDT::Install();

	Kernel::Terminal::WriteString("Installed GDT\n");

	Kernel::Memory::TSS::Install();

	Kernel::Terminal::WriteString("Installed TSS\n");

	Kernel::Memory::IDT::Init();
	Kernel::Memory::IDT::Install();

	Kernel::Terminal::WriteString("Installed IDT\n");

	Kernel::Memory::IRQ::Install();

	Kernel::Terminal::WriteString("Installed IRQ\n");

	Kernel::Memory::ISR::Install();
	
	Kernel::Terminal::WriteString("Installed ISR\n");

	Kernel::Memory::Paging::Init();

	Kernel::Terminal::WriteString("Installed Paging\n");

	// This is wrong
	// Kernel::Multiboot::ParsedMultibootInfo multiboot_info = Kernel::Multiboot::ParseTags(multiboot_info_address);



	for (;;)
		asm volatile("hlt");
}
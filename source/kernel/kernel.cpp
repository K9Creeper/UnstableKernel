#include "terminal/terminal.hpp"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"
#include "memory/task_state_segment/task_state_segment.hpp"

#include "memory/interrupt_service_routines/interrupt_service_routines.hpp"
#include "memory/interrupt_request/interrupt_request.hpp"

#include "memory/physical_memory_manager/physical_memory_manager.hpp"
#include "memory/paging/paging.hpp"

extern "C" void jump_usermode(void);

extern "C" void switch_to_real_mode(void);
extern "C" void switch_to_protected_mode(void);

extern "C" void real_mode_handle(void)
{
	Kernel::Terminal::WriteString("The most unstable kernel you'll ever see.");
}

extern uint32_t linkerld_startofkernel;

// Look here: https://github.com/JetStarBlues/JamesM-Kernel-Tutorial

extern "C" void kernel_main(void)
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

	uint32_t cr0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	if (!(cr0 & 0x80000000))
	{
		Kernel::Terminal::WriteString("Paging not enabled!\n");
	}
	else
	{
		Kernel::Terminal::WriteString("Paging enabled successfully!\n");
	}

	// Check if kernelDirectory is non-null
	if (Kernel::Memory::Paging::kernelDirectory)
	{
		Kernel::Terminal::WriteString("Page directory initialized.\n");
	}
	else
	{
		Kernel::Terminal::WriteString("Failed to initialize page directory.\n");
	}

	for (;;)
		asm volatile("hlt");
}
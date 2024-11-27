#include "terminal/terminal.hpp"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"
#include "memory/task_state_segment/task_state_segment.hpp"

#include "memory/interrupt_service_routines/interrupt_service_routines.hpp"
#include "memory/interrupt_request/interrupt_request.hpp"

#include "memory/physical_memory_manager/physical_memory_manager.hpp"
#include "memory/paging/paging.hpp"
#include "memory/kheap/kheap.hpp"

extern "C" void jump_usermode(void);

extern "C" void switch_to_real_mode(void);
extern "C" void switch_to_protected_mode(void);

extern "C" void real_mode_handle(void){
	Kernel::Terminal::WriteString("The most unstable kernel you'll ever see.");
}

extern uint32_t linkerld_startofkernel;

// Look here: https://github.com/JetStarBlues/JamesM-Kernel-Tutorial

extern "C" void kernel_main(void) 
{
	Kernel::Terminal::Init();
	
	Kernel::Memory::GDT::Init();
	Kernel::Memory::GDT::Install();

	Kernel::Memory::IDT::Init();
	Kernel::Memory::IDT::Install();

	Kernel::Memory::ISR::Install();
	Kernel::Memory::IRQ::Install();

    // causes fault 13
	// asm volatile("sti");

	Kernel::Memory::Paging::Init();

	for(;;)
		asm volatile("hlt");
}
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

extern "C" void real_mode_handle(void){
	Kernel::Terminal::WriteString("The most unstable kernel you'll ever see.");
}

extern "C" void kernel_main(void) 
{
	Kernel::Terminal::Init();
	
	Kernel::Memory::GDT::Init();
	Kernel::Memory::GDT::Install();

	Kernel::Memory::IDT::Init();
	Kernel::Memory::IDT::Install();

	Kernel::Memory::TSS::Install();

	Kernel::Memory::PMM::Init(1096 * (1024 * 1024));

	Kernel::Memory::Paging::Init();

	Kernel::Memory::ISR::Install();
  	Kernel::Memory::IRQ::Install();
	
	// switch_to_real_mode();

	// switch_to_protected_mode();

	// jump_usermode();

	Kernel::Terminal::WriteString("Hi!");

	for(;;)
		asm volatile("hlt");
}
#include "terminal/terminal.hpp"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"
#include "memory/task_state_segment/task_state_segment.hpp"

#include "memory/interrupt_service_routines/interrupt_service_routines.hpp"
#include "memory/interrupt_request/interrupt_request.hpp"

extern "C" void kernel_main(void) 
{
	Kernel::Terminal::Clear();
	Kernel::Terminal::WriteString("The most unstable kernel you'll ever see.");
	
	Kernel::Memory::GDT::Init();
	Kernel::Memory::GDT::Install();

	Kernel::Memory::IDT::Init();
	Kernel::Memory::IDT::Install();

	Kernel::Memory::TSS::Install();

	Kernel::Memory::ISR::Install();
  	Kernel::Memory::IRQ::Install();

	for(;;)
		asm volatile("hlt");
}
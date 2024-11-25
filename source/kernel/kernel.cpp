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

extern uint32_t linkerld_endofkernel;

void Test(){
	char* buffer = reinterpret_cast<char*>(Kernel::Memory::KHeap::Malloc(256));

	for(int i = 33; i < 126; i++)
	{
		buffer[i-33] = static_cast<char>(i);
	}

	Kernel::Terminal::WriteString(buffer);

	Kernel::Memory::KHeap::Free(buffer);

}

// Take a look here: https://wiki.osdev.org/Higher_Half_x86_Bare_Bones

extern "C" void kernel_main(void) 
{
	Kernel::Memory::KHeap::Init(linkerld_endofkernel);

	Kernel::Terminal::Init();
	
	Kernel::Memory::GDT::Init();
	Kernel::Memory::GDT::Install();

	Kernel::Memory::IDT::Init();
	Kernel::Memory::IDT::Install();

	asm volatile("sti");

	Kernel::Terminal::WriteString("Past STI\n");

	for(;;)
		asm volatile("hlt");
}
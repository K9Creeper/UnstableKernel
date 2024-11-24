#include "terminal/terminal.hpp"

extern "C" void kernel_main(void) 
{
	asm volatile("cli");

	Kernel::Terminal::Clear();
	Kernel::Terminal::WriteString("Hello World!\n");

	for(;;)
		asm volatile("hlt");
}
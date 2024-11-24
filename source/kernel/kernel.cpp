#include "terminal/terminal.hpp"

extern "C" void kernel_main(void) 
{
	Kernel::Terminal::Clear();
}
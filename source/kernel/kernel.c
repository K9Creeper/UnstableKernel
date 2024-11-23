void kernel_entry(void) 
{
	for(;;)
		asm volatile("hlt");
}
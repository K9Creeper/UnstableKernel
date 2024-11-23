extern void kernel_entry()
{
  for(;;)
      asm volatile ("hlt");
}
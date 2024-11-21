/// kernel.c

void kernel_entry(void* boot_info){
    for(;;)
      asm volatile ("hlt");
}
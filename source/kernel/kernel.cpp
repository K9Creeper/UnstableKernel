#include "../c_helpers/multiboot.h"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"

#include "memory/kheap/kheap.hpp"
#include "memory/paging/paging.hpp"

#include "multiboot/multiboot.hpp"

#include "input/keyboard.hpp"

#include "debug\serial.hpp"

#include "graphics/graphics.hpp"

extern "C" void jump_usermode(void);

extern uint32_t linkerld_startofkernel;

extern "C" uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t *physAddress);
extern "C" void kfree_(void* ptr);

void HeapTest(){
    char* buffer;
    kmalloc_(sizeof(char)*255, 0, 0);
    buffer[0] = 'D';
    buffer[1] = 'D';
    buffer[2] = 'D';
    buffer[3] = 'D';
    buffer[4] = 'D';
    buffer[5] = 'D';

    kfree_(buffer);
}



extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    Kernel::Debug::COM1::Init();

    multiboot_tag_framebuffer_common *fbc = Kernel::Multiboot::GetFrameBuffer(addr);
    
    Kernel::Graphics::Init(fbc->framebuffer_addr, fbc->size, fbc->framebuffer_width, fbc->framebuffer_height, fbc->framebuffer_pitch, fbc->framebuffer_bpp);

    Kernel::Memory::GDT::Init();
    Kernel::Memory::GDT::Install();

    Kernel::Memory::IDT::Init();

    Kernel::Memory::IDT::Install();

    Kernel::Memory::Paging::Init(KHEAP_START, Kernel::Graphics::FrameBuffer::addr, Kernel::Graphics::FrameBuffer::size);

    Kernel::Memory::KHeap::Init(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, false, false);

    Kernel::Input::Keyboard::Init();

    // Enable interrupts
    asm volatile("sti");

    HeapTest();

    for(int i = 0; i < 100; i++)
    {
        Kernel::Graphics::SetPixel(i, i, 255);
    }

    for (;;)
        asm volatile("hlt");
}
#include "../c_helpers/multiboot.h"

#include "memory/global_descriptor_table/global_descriptor_table.hpp"
#include "memory/interrupt_descriptor_table/interrupt_descriptor_table.hpp"
#include "memory/task_state_segment/task_state_segment.hpp"

#include "memory/kheap/kheap.hpp"
#include "memory/paging/paging.hpp"

#include "multiboot/multiboot.hpp"

#include "input/keyboard.hpp"

#include "debug/serial.hpp"

#include "graphics/graphics.hpp"

extern "C" void jump_usermode(void);

extern "C" uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t *physAddress);
extern "C" void kfree_(void *ptr);

extern "C" void kernel_main(uint32_t addr, uint32_t magic)
{
    Kernel::Debug::COM1::Init();

    multiboot_tag_framebuffer_common *fbc = Kernel::Multiboot::GetFrameBuffer(addr);

    Kernel::Graphics::Init(0xA00000, fbc->framebuffer_height * fbc->framebuffer_width * (fbc->framebuffer_bpp / 8), fbc->framebuffer_width, fbc->framebuffer_height, fbc->framebuffer_pitch, fbc->framebuffer_bpp);

    printf("Framebuffer at 0x%X\n", Kernel::Graphics::FrameBuffer::addr);

    // shows on screen
    for (int i = 50; i < 100; i++)
    {
        Kernel::Graphics::SetPixel(i, i, 1);
    }

    Kernel::Memory::GDT::Init();
    Kernel::Memory::GDT::Install();

    // Kernel::Memory::TSS::Install();

    Kernel::Memory::IDT::Init();

    Kernel::Memory::IDT::Install();

    Kernel::Memory::Paging::Init(Kernel::Graphics::FrameBuffer::addr, Kernel::Graphics::FrameBuffer::size);

    for (int i = 100; i < 150; i++)
    {
        Kernel::Graphics::SetPixel(i, i, 7);
    }

    Kernel::Memory::Paging::SwitchPageDirectory(Kernel::Memory::Paging::kernelDirectory);
    Kernel::Memory::Paging::Enable();
    printf("TEST1\n");
    Kernel::Memory::KHeap::Init(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX_END, false, false);
    printf("TEST2\n");
    Kernel::Input::Keyboard::Init();

    // Enable interrupts
    asm volatile("sti");

    printf("TEST3\n");
    for (int i = 0; i < 50; i++)
    {
        Kernel::Graphics::SetPixel(i, i, 4);
    }
    printf("TEST4\n");

    for (;;)
        asm volatile("hlt");
}
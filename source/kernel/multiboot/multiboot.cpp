#include "multiboot.hpp"

#include "../memory/kheap/kheap.hpp"

multiboot_tag_framebuffer_common *Kernel::Multiboot::GetFrameBuffer(uint32_t addr) {
    // Iterate through the tags in the multiboot info structure
    for (struct multiboot_tag* tag = (struct multiboot_tag*) (addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END; 
        tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7))) {
        // Check if the tag is the framebuffer tag
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
            return (multiboot_tag_framebuffer_common *)tag;
        }
    }
    return nullptr;  // Return NULL if no framebuffer tag is found
}

extern uint32_t linkerld_startofkernel;

void Kernel::Multiboot::Load(uint32_t addr, uint32_t magic)
{
}
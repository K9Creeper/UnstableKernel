#include "multiboot.hpp"

namespace Kernel
{
    namespace Multiboot
    {
        uint32_t mb_info = 0;
        uint32_t mb_magic = 0;
    }
}
multiboot_tag_framebuffer_common *Kernel::Multiboot::GetFrameBuffer()
{
    // Iterate through the tags in the multiboot info structure
    for (multiboot_tag *tag = reinterpret_cast<multiboot_tag *>(mb_info + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = reinterpret_cast<multiboot_tag *>(reinterpret_cast<multiboot_uint8_t *>(tag) + ((tag->size + 7) & ~7)))
    {
        // Check if the tag is the framebuffer tag
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER)
        {
            return reinterpret_cast<multiboot_tag_framebuffer_common *>(tag);
        }
    }
    return nullptr; // Return nullptr if no framebuffer tag is found
}

multiboot_tag_basic_meminfo *Kernel::Multiboot::GetBasicMemInfo()
{
    // Iterate through the tags in the multiboot info structure
    for (multiboot_tag *tag = reinterpret_cast<multiboot_tag *>(mb_info + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = reinterpret_cast<multiboot_tag *>(reinterpret_cast<multiboot_uint8_t *>(tag) + ((tag->size + 7) & ~7)))
    {
        // Check if the tag is the basic memory info tag
        if (tag->type == MULTIBOOT_TAG_TYPE_BASIC_MEMINFO)
        {
            return reinterpret_cast<multiboot_tag_basic_meminfo *>(tag);
        }
    }
    return nullptr; // Return nullptr if no memory info tag is found
}
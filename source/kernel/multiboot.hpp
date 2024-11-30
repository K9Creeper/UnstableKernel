#pragma once

#include <stdint.h>

#include "../c_helpers/multiboot.h"
#include "../c_helpers/ordered_array.h"
namespace Kernel
{
    namespace Multiboot
    {
        struct ModuleInfo
        {
            unsigned mod_start;
            unsigned mod_end;
            char* cmdline;
        };

        struct MmapEntry
        {
            uint64_t base_addr;
            uint64_t length;
            uint32_t type;
        };

        struct FramebufferInfo
        {
            uint32_t addr;
            uint32_t width;
            uint32_t height;
            uint32_t bpp;
        };

        class ParsedMultibootInfo
        {
            public:

            ~ParsedMultibootInfo();

            char* cmdline;
            char* bootloader_name;
            // ModuleInfo* 
            ordered_array_t modules;
            uint32_t mem_lower = 0;
            uint32_t mem_upper = 0;
            uint32_t boot_device = 0;
            // MmapEntry* 
            ordered_array_t mmap_entries;
            FramebufferInfo framebuffer;
        };

        extern ParsedMultibootInfo ParseTags(uint32_t addr);
    }
}
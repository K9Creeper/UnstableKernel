#pragma once

#include <stdint.h>

#include "../../c_helpers/multiboot.h"

namespace Kernel
{
    namespace Multiboot
    {
        // Take a look here: https://github.com/Phil0nator/OS/blob/main/src/impl/kernel/multiboot_loader.c
        extern multiboot_tag_framebuffer_common * GetFrameBuffer(uint32_t mb_info);

        extern void Load(uint32_t addr, uint32_t magic);
    }
}
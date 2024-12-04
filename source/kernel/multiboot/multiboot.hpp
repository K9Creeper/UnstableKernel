#pragma once

#include <stdint.h>

#include "../../chelpers/multiboot.h"

namespace Kernel
{
    namespace Multiboot
    {
        extern uint32_t mb_info;
        extern uint32_t mb_magic;

        // Take a look here: https://github.com/Phil0nator/OS/blob/main/src/impl/kernel/multiboot_loader.c
        extern multiboot_tag_framebuffer_common * GetFrameBuffer();
        extern multiboot_tag_basic_meminfo* GetBasicMemInfo();
    }
}
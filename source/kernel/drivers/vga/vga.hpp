#pragma once

#include <stdint.h>

namespace Kernel{
    namespace Drivers{
        namespace VGA{
            extern uint32_t vga_buffer_address;

            extern uint32_t height;
            extern uint32_t width;

            extern void Init();

            extern void Clear();

            extern void printf(const char * format, ...);
        }
    }
}
#pragma once

#include <stdint.h>

namespace Kernel
{
    namespace Graphics
    {
        namespace FrameBuffer
        {
            extern uint32_t addr;

            extern uint32_t size;
            
            extern uint32_t width;
            extern uint32_t height;

            extern uint32_t pitch;
            extern uint8_t bpp;
        }

        extern bool bInitialized;

        extern void Init(uint32_t addr, uint32_t size, uint32_t width, uint32_t height, uint32_t pitch, uint8_t bpp);

        extern uint32_t GetPixel(uint32_t x, uint32_t y);
        extern void SetPixel(uint32_t x, uint32_t y, uint32_t val);
    }
}
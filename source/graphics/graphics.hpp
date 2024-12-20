/// ------------
/// graphics.hpp
/// @brief

#pragma once

#include <stdint.h>

#include "framebuffer.hpp"

namespace Graphics{
    extern Framebuffer<uint32_t> buffer1;

    extern void Init(uint32_t pBuff1, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp);
    
    extern void SetPixel(uint32_t x, uint32_t y, uint32_t color);

    extern void Swap();
}
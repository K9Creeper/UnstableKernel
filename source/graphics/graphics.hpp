/// ------------
/// graphics.hpp
/// @brief This file declares the data or information for system graphics.
#pragma once

#include <stdint.h>

namespace Graphics{
    extern uint32_t pxWidth;
    extern uint32_t pxHeight;
    extern uint32_t pitch;
    extern uint32_t bPerPixel;

    extern void Init(uint32_t lfbLocation, uint32_t pxWidth, uint32_t pxHeight, uint32_t pitch, uint32_t bPerPixel);
    extern void FillBuffer(uint32_t color);
    extern void Paint(uint32_t x, uint32_t y, uint32_t color);
    extern void SwapBuffers(bool clear = true);
}
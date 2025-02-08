/// ---------------
/// framebuffer.hpp
/// @brief This file declares the framebuffer class.
#pragma once

#include <stdint.h>

class Framebuffer{
private:
    uint32_t* data;
    
    uint32_t width;
    uint32_t height;

    uint32_t pitch;
    uint32_t bpp;

public:
    void Set(uint32_t* src, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp);

    void Clear();

    void Swap(Framebuffer* other);

    void PutPixel(uint32_t x, uint32_t y, uint32_t color);

    uint32_t GetSize()const;
private:
    
};
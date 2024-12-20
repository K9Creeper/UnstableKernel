/// ------------
/// graphics.cpp
/// @brief

#include "graphics.hpp"

namespace Graphics{
    Framebuffer<uint32_t> buffer1;
}

void Graphics::Init(uint32_t pBuff1, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp){
    buffer1.RePlace(pBuff1);

    buffer1.width = width;
    buffer1.height = height;
    buffer1.pitch = pitch;
    buffer1.bpp = bpp;
}

void Graphics::SetPixel(uint32_t x, uint32_t y, uint32_t color){
    if(x < 0 || x >= buffer1.width || y < 0 || y >= buffer1.height)
        return;
    uint32_t* loc = reinterpret_cast<uint32_t*>(y * buffer1.pitch + (x * (buffer1.bpp / 8)) + buffer1.GetDataLocation());
    *loc = color;
}

void Graphics::Swap(){
    // ya don't worry about this
}
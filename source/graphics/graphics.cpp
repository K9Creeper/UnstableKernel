/// ------------
/// graphics.cpp
/// @brief This file defines the data / information for system graphics.

#include "graphics.hpp"

#include "../kernel/memory_management/kheap/kheap.hpp"

namespace Graphics{
    uint32_t pxWidth;
    uint32_t pxHeight;
    uint32_t pitch;
    uint32_t bPerPixel;

    Framebuffer foregroundBuffer;
    Framebuffer backBuffer;
}

extern "C" void printf(const char* format, ...);

void Graphics::Init(uint32_t lfbLocation, uint32_t w, uint32_t h, uint32_t p, uint32_t b){
    pxWidth = w;
    pxHeight = h;
    pitch = p;
    bPerPixel = b;

    foregroundBuffer.Init(reinterpret_cast<uint32_t*>(lfbLocation), pxWidth, pxHeight, pitch, bPerPixel);
    backBuffer.Init(reinterpret_cast<uint32_t*>(Kernel::MemoryManagement::kheap.malloc_(foregroundBuffer.GetSize())), pxWidth, pxHeight, pitch, bPerPixel);
}

void Graphics::FillBuffer(uint32_t color){
    for(uint32_t y = 0; y < pxHeight; y++)
        for(uint32_t x = 0; x < pxWidth; x++)
            Graphics::Paint(x, y, color);
}

void Graphics::Paint(uint32_t x, uint32_t y, uint32_t color, uint8_t opacity) {
    uint32_t* pixel = backBuffer.GetPixel(x, y);
    if (pixel) {
        if (opacity == 255) {
            (*pixel) = color;
        } else {
            uint8_t srcR = (color >> 16) & 0xFF;
            uint8_t srcG = (color >> 8) & 0xFF;
            uint8_t srcB = color & 0xFF;
            
            uint8_t dstR = (*pixel >> 16) & 0xFF;
            uint8_t dstG = (*pixel >> 8) & 0xFF;
            uint8_t dstB = (*pixel) & 0xFF;

            uint8_t outR = ((srcR * opacity) + (dstR * (255 - opacity))) / 255;
            uint8_t outG = ((srcG * opacity) + (dstG * (255 - opacity))) / 255;
            uint8_t outB = ((srcB * opacity) + (dstB * (255 - opacity))) / 255;

            *pixel = (outR << 16) | (outG << 8) | outB;
        }
    }
}

Graphics::Framebuffer* Graphics::GetBackBuffer(){
    return &backBuffer;
}

void Graphics::SwapBuffers(bool clear){
    foregroundBuffer.SwapTo(&backBuffer);
    if(clear)
        backBuffer.Clear();
}
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
    backBuffer.Init(reinterpret_cast<uint32_t*>(Kernel::MemoryManagement::KHeap::kmalloc_(foregroundBuffer.GetSize())), pxWidth, pxHeight, pitch, bPerPixel);
}

void Graphics::FillBuffer(uint32_t color){
    for(uint32_t y = 0; y < pxHeight; y++)
        for(uint32_t x = 0; x < pxWidth; x++)
            Graphics::Paint(x, y, color);
}

void Graphics::Paint(uint32_t x, uint32_t y, uint32_t color){
    uint32_t* pixel = backBuffer.GetPixel(x, y);
    if(pixel)
        (*pixel) = color;
}

Framebuffer* Graphics::GetBackBuffer(){
    return &backBuffer;
}

void Graphics::SwapBuffers(bool clear){
    foregroundBuffer.SwapTo(&backBuffer);
    if(clear)
        backBuffer.Clear();
}
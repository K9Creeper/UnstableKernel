/// -----------
/// desktop.cpp
/// @brief This file defines the function relating to drawing the desktop.

#include "style.hpp"

#include "../../../graphics/framebuffer.hpp"

namespace Usermode
{
    namespace Graphics
    {
        namespace Native
        {
            void DrawDesktop(::Graphics::Framebuffer* fb);
        }
    }
}

static void PutPixel(::Graphics::Framebuffer* fb, uint32_t x, uint32_t y, uint32_t color){
    if(uint32_t* p = fb->GetPixel(x, y))
        *p = color;
}

static void FillBuffer(::Graphics::Framebuffer* fb, uint32_t color){
    uint32_t w = fb->GetWidth();
    uint32_t h = fb->GetHeight();

    for(uint32_t y = 0; y < h; y++)
        for(uint32_t x = 0; x < w; x++)
            PutPixel(fb, x, y, color);
}

void Usermode::Graphics::Native::DrawDesktop(::Graphics::Framebuffer* fb){
    FillBuffer(fb, Style::desktopBkgColor);
}
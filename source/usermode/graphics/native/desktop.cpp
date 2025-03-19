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

static void PutPixel(::Graphics::Framebuffer *fb, int x, int y, uint32_t color, uint8_t opacity = 255)
{
    uint32_t *pixel = fb->GetPixel(x, y);
    if (pixel)
    {
        if (opacity == 255)
        {
            (*pixel) = color;
        }
        else
        {
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

static void DrawBox(::Graphics::Framebuffer* fb, int l, int t, int r, int b, uint32_t color, uint8_t opacity = 255){
    for(int y = t; y <= b; y++)
        for(int x = l; x <= r; x++)
            PutPixel(fb, x, y, color, opacity);
}

static void FillBuffer(::Graphics::Framebuffer* fb, uint32_t color){
    uint32_t w = fb->GetWidth();
    uint32_t h = fb->GetHeight();

    for(uint32_t y = 0; y < h; y++)
        for(uint32_t x = 0; x < w; x++)
            PutPixel(fb, x, y, color);
}



void Usermode::Graphics::Native::DrawDesktop(::Graphics::Framebuffer* fb){
    // Lets draw bkg
    FillBuffer(fb, Style::desktopBkgColor);

    // Lets draw a bottom navbar
    DrawBox(fb, 0, fb->GetHeight() - static_cast<int>(Style::desktopNavBarHeightRatio * fb->GetHeight()), fb->GetWidth(), fb->GetHeight(), Style::desktopNavBarColor, Style::desktopNavBarOpacity);
}
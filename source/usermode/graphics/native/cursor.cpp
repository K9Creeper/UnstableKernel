/// -----------
/// desktop.cpp
/// @brief This file defines the function relating to drawing the cursor.

#include "style.hpp"

#include "../../../graphics/framebuffer.hpp"

#include "../../input/input.hpp"

namespace Usermode
{
    namespace Graphics
    {
        namespace Native
        {
            void DrawCursor(::Graphics::Framebuffer *fb);
        }
    }
}

static void PutPixel(::Graphics::Framebuffer *fb, uint32_t x, uint32_t y, uint32_t color)
{
    if (uint32_t *p = fb->GetPixel(x, y))
        *p = color;
}

void Usermode::Graphics::Native::DrawCursor(::Graphics::Framebuffer *fb)
{
    if (!Input::bInitialized)
        return;

    uint32_t x = Input::mouseInfo.X;
    uint32_t y = Input::mouseInfo.Y;

    uint32_t centerX = x + Style::mouseCursorSize / 2;
    uint32_t centerY = y + Style::mouseCursorSize / 2;
    int thickness = Style::mouseCursorSize / Style::mouseCursorSize;

    for (int i = -thickness; i <= thickness; ++i)
    {
        for (int j = 0; j < Style::mouseCursorSize; ++j)
        {
            PutPixel(fb, centerX + i, y + j, Style::mouseCursorColor);
        }
    }

    for (int i = -thickness; i <= thickness; ++i)
    {
        for (int j = 0; j < Style::mouseCursorSize; ++j)
        {
            PutPixel(fb, x + j, centerY + i, Style::mouseCursorColor);
        }
    }
}
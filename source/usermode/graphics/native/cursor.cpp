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

static void PutPixel(::Graphics::Framebuffer *fb, int x, int y, uint32_t color)
{
    if (uint32_t *p = fb->GetPixel(x, y))
        *p = color;
}

void Usermode::Graphics::Native::DrawCursor(::Graphics::Framebuffer *fb)
{
    if (!Input::bInitialized)
        return;

    int x = Input::mouseInfo.X;
    int y = Input::mouseInfo.Y;

    int thickness = 1;

    int half = (Style::mouseCursorSize/2);

    for (int i = -thickness; i <= thickness; ++i)
    {        
        for (int j = 0; j < Style::mouseCursorSize; ++j)
        {
            uint32_t color = (j == 0 || j == Style::mouseCursorSize - 1) ? 0x0 : Style::mouseCursorColor;

            PutPixel(fb, x + i, y - half + j, color);

            PutPixel(fb, x - half + j, y + i, color);
        }
    }
}
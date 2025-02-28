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

    int x = Input::mouseInfoBuffer.X;
    int y = Input::mouseInfoBuffer.Y;

    int half = (Style::mouseCursorSize / 2);

    for (int j = 0; j < Style::mouseCursorSize; ++j)
    {
        PutPixel(fb, x, y - half + j, Style::mouseCursorColor);
        PutPixel(fb, x - half + j, y, Style::mouseCursorColor);
    }
}
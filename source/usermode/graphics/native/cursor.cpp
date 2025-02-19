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
            void DrawCursor(::Graphics::Framebuffer* fb);
        }
    }
}

static void PutPixel(::Graphics::Framebuffer* fb, uint32_t x, uint32_t y, uint32_t color){
    if(uint32_t* p = fb->GetPixel(x, y))
        *p = color;
}

extern "C" void printf(const char* f, ...);

void Usermode::Graphics::Native::DrawCursor(::Graphics::Framebuffer* fb){
    if(!Input::bInitialized)
        return;

    uint32_t arrowSize = Style::mouseCursorSize / 4;

    uint32_t x = Input::mouseInfo.X;
    uint32_t y = Input::mouseInfo.Y;

    for (int i = 0; i < Style::mouseCursorSize; i++) {
        PutPixel(fb, x, y + i, Style::mouseCursorColor);  // Shaft of the arrow
    }

    for (int i = 0; i < arrowSize; i++) {
        PutPixel(fb, x - i, y + i, Style::mouseCursorColor); // Left side of the arrowhead
        PutPixel(fb, x + i, y + i, Style::mouseCursorColor); // Right side of the arrowhead
    }

}
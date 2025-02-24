/// ----------
/// window.hpp
/// @brief This file defines the Window structure.

#pragma once

#define WS_WINDOWED 0x10000U
#define WS_BORDERLESS 0x20000U
#define WS_FULLSCREEN 0x30000U

#define WS_SHOW_TITLE_BAR 0x00010U
#define WS_SHOW_TOOLS 0x00020U

#include "../../../../graphics/framebuffer.hpp"

namespace Usermode
{
    namespace Graphics
    {
        namespace Windows
        {
            struct Window
            {
                // window name
                char name[512];

                uint16_t zindex;

                // left, top, right, bottom
                int l, t, r, b;

                // How to render the window
                uint32_t windowStyleFlags = WS_WINDOWED | WS_SHOW_TOOLS | WS_SHOW_TITLE_BAR;

                uint32_t width, height;

                struct Viewport
                {
                    // left, top, right, bottom
                    int l, t, r, b;
                } viewport;

                // our framebuffer, includes ONLY the viewport
                ::Graphics::Framebuffer framebuffer;
            };
        }
    }
}
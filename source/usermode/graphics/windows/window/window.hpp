/// ----------
/// window.hpp
/// @brief This file defines the Window structure.

#pragma once

//
// STYLE FLAGS 
//

#define WS_WINDOWED 0x10000U
#define WS_BORDERLESS 0x20000U
#define WS_FULLSCREEN 0x30000U

#define WS_SHOW_TITLE_BAR 0x00010U
#define WS_SHOW_TOOLS 0x00020U

//
// WINDOW MSGS (CALLBACKS)
//

#define WM_DESTROY 0U
#define WM_MINIMIZED 1U
#define WM_PAINT 2U

#define WM_KEYDOWN 3U // like held or on up

#define WM_KEYUP 4U // on up only (duuh)

#define WM_MB_L_DOWN 5U // like held or on up
#define WM_MB_L_UP 6U // on up only (duh)

#define WM_MB_R_DOWN 7U // like held or on up
#define WM_MB_R_UP 8U // on up only (duh)

#define WM_M_MOVE 9U


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

                uint16_t zindex = 0;

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

                uint32_t (*callback)(Window* w, uint8_t msg) = nullptr;
            };
        }
    }
}
/// -----------
/// windows.hpp
/// @brief This file declares the function to initiate window rendering.

#pragma once

#include "window/window.hpp"

#include "../../../graphics/framebuffer.hpp"

namespace Usermode
{
    namespace Graphics
    {
        namespace Windows
        {
            extern bool bInitialized;

            extern Usermode::Graphics::Windows::Window* CreateWindow(const char* windowName, uint32_t windowFlags, 
                uint32_t x, uint32_t y, 
                uint32_t width, uint32_t height, 
                void* callback = nullptr,
                bool focus = true);
        }
    }
}
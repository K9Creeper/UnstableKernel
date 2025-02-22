/// -----------
/// windows.hpp
/// @brief This file declares the function to initiate window rendering.

#pragma once

#include "../../../graphics/framebuffer.hpp"

namespace Usermode
{
    namespace Graphics
    {
        namespace Windows
        {
            extern bool bInitialized;
            extern void Init();
            extern void RenderWindows(::Graphics::Framebuffer* fb);
        }
    }
}
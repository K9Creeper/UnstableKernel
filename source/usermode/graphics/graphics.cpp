/// ------------
/// graphics.cpp
/// @brief This file defines the functions that handle the OS graphics.

#include "graphics.hpp"

#include "../../graphics/graphics.hpp"
#include "../../kernel/multitasking/syscall.hpp"

namespace Usermode{
    namespace Graphics{
        bool bThreadRunning = false;

        namespace Native{
            extern void DrawDesktop(::Graphics::Framebuffer* fb);
            extern void DrawCursor(::Graphics::Framebuffer* fb);
        }

    }
}


void Usermode::Graphics::Thread()
{
    bThreadRunning = true;

    while(bThreadRunning){
        ::Graphics::Framebuffer* fb = ::Graphics::GetBackBuffer();

        if(!fb)
            continue;

        Native::DrawDesktop(fb);

        

        // We want to draw this above everything else...
        Native::DrawCursor(fb);

        ::Graphics::SwapBuffers();
    }

    sys_exit();
}
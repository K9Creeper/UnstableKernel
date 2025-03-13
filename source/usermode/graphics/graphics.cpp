/// ------------
/// graphics.cpp
/// @brief This file defines the functions that handle the OS graphics.

#include "graphics.hpp"

#include "../input/input.hpp"

#include "../../graphics/graphics.hpp"
#include "../../kernel/multitasking/syscall.hpp"

#include "windows/windows.hpp"

namespace Usermode{
    namespace Graphics{
        bool bThreadRunning = false;

        ::Graphics::Framebuffer* currentFb;

        namespace Native{
            extern void DrawDesktop(::Graphics::Framebuffer* fb);
            extern void DrawCursor(::Graphics::Framebuffer* fb);
        }

        namespace Windows{
            extern void Init();
            extern void RenderWindows(::Graphics::Framebuffer* fb);
            extern void HandleDragFocusInput();
        }

        void DrawAndSwap();
    }
}

uint32_t Usermode::Graphics::GetScreenX(){
    return Usermode::Graphics::currentFb->GetWidth();
}

uint32_t Usermode::Graphics::GetScreenY(){
    return Usermode::Graphics::currentFb->GetHeight();
}

uint32_t Usermode::Graphics::GetScreenPitch(){
    return Usermode::Graphics::currentFb->GetPitch();
}

uint32_t Usermode::Graphics::GetScreenBPP(){
    return Usermode::Graphics::currentFb->GetBPP();
}

void Usermode::Graphics::DrawAndSwap(){
    // This is the background...    
    Native::DrawDesktop(currentFb);

    Usermode::Graphics::Windows::RenderWindows(currentFb);

    // We want to draw this above everything else...
    Native::DrawCursor(currentFb);

    // Swap Buffers
    ::Graphics::SwapBuffers();
}

static void HandleInput(){
    Usermode::Input::mouseInfoBuffer = Usermode::Input::mouseInfo;
    if(Usermode::Input::keyboardMap)
        Usermode::Input::keyboardMapBuffer = *Usermode::Input::keyboardMap;

    Usermode::Graphics::Windows::HandleDragFocusInput();
}

void Usermode::Graphics::Thread()
{
    Usermode::Graphics::Windows::Init();

    bThreadRunning = true;

    currentFb = ::Graphics::GetBackBuffer();

    Usermode::Graphics::Windows::CreateWindow("Test1", 0, 200, 200, 100, 100);
    Usermode::Graphics::Windows::CreateWindow("Test2", 0, 400, 200, 100, 100);

    while(bThreadRunning){
        HandleInput();

        if(!currentFb){
            currentFb = ::Graphics::GetBackBuffer();
            continue;
        }

        // Render
        DrawAndSwap();        
    }

    sys_exit();
}
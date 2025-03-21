/// ------------
/// graphics.cpp
/// @brief This file defines the functions that handle the OS graphics.

#include "graphics.hpp"

#include "../input/input.hpp"

#include "../../graphics/graphics.hpp"
#include "../../kernel/multitasking/syscall.hpp"

#include "windows/windows.hpp"
#include "windows/window_draw_list/window_draw_list.hpp"

namespace Usermode
{
    namespace Graphics
    {
        bool bThreadRunning = false;

        ::Graphics::Framebuffer *currentFb;

        namespace Native
        {
            extern void DrawDesktop(::Graphics::Framebuffer *fb);
            extern void DrawCursor(::Graphics::Framebuffer *fb);
        }

        namespace Windows
        {
            extern WindowDrawList windowDrawList;

            extern void Init();
            extern void RenderWindows(::Graphics::Framebuffer *fb);
            extern void HandleDragFocusInput();
        }

        void DrawAndSwap();

        MouseInfo mouseInfoBuffer;
    }
}

uint32_t Usermode::Graphics::GetScreenX()
{
    return Usermode::Graphics::currentFb->GetWidth();
}

uint32_t Usermode::Graphics::GetScreenY()
{
    return Usermode::Graphics::currentFb->GetHeight();
}

uint32_t Usermode::Graphics::GetScreenPitch()
{
    return Usermode::Graphics::currentFb->GetPitch();
}

uint32_t Usermode::Graphics::GetScreenBPP()
{
    return Usermode::Graphics::currentFb->GetBPP();
}

void Usermode::Graphics::DrawAndSwap()
{
    // This is the background...
    Native::DrawDesktop(currentFb);

    Usermode::Graphics::Windows::RenderWindows(currentFb);

    // We want to draw this above everything else...
    Native::DrawCursor(currentFb);

    // Swap Buffers
    ::Graphics::SwapBuffers();
}

static void GMouseHandle(const MouseInfo &info)
{
    Usermode::Graphics::mouseInfoBuffer = info;

    Usermode::Graphics::Windows::HandleDragFocusInput();

    if (Usermode::Graphics::Windows::Window *window = Usermode::Graphics::Windows::windowDrawList.GetFocusedWindow())
    {
        if (window->callback)
        {
            if (Usermode::Graphics::mouseInfoBuffer.currState[0])
            {
                window->callback(window, WM_MB_L_DOWN);
            }

            if (!Usermode::Graphics::mouseInfoBuffer.currState[0] && Usermode::Graphics::mouseInfoBuffer.prevState[0])
            {
                window->callback(window, WM_MB_L_UP);
            }

            if (Usermode::Graphics::mouseInfoBuffer.currState[1])
            {
                window->callback(window, WM_MB_R_DOWN);
            }

            if (!Usermode::Graphics::mouseInfoBuffer.currState[1] && Usermode::Graphics::mouseInfoBuffer.prevState[1])
            {
                window->callback(window, WM_MB_R_UP);
            }

            if (Usermode::Graphics::mouseInfoBuffer.CHANGE_X || Usermode::Graphics::mouseInfoBuffer.CHANGE_Y)
                window->callback(window, WM_M_MOVE);
        }
    }
}

static void GKeyboardHandle(const KeyboardKey &k, const KeyboardKey *keymap)
{
    if (Usermode::Graphics::Windows::Window *window = Usermode::Graphics::Windows::windowDrawList.GetFocusedWindow())
    {
        if (window->callback)
        {
            if(k.bPressed)
            {
                window->callback(window, WM_KEYDOWN);
            }

            if(!k.bPressed && k.bPressedPrev)
            {
                window->callback(window, WM_KEYUP);
            }
        }
    }
}

static uint32_t WndProc(Usermode::Graphics::Windows::Window* window, uint8_t msg){

    switch(msg)
    {
        case WM_DESTROY:

            break;
    }


    return 0;
}

void Usermode::Graphics::Thread()
{
    int m = Usermode::Input::AddMouseHandle(GMouseHandle);
    int k = Usermode::Input::AddKeyboardHandle(GKeyboardHandle);

    Usermode::Graphics::Windows::Init();

    bThreadRunning = true;

    currentFb = ::Graphics::GetBackBuffer();

    Usermode::Graphics::Windows::CreateWindow("WindowTest1", 0, 200, 200, 150, 100, WndProc);
    Usermode::Graphics::Windows::CreateWindow("WindowTest2", 0, 400, 200, 100, 150, WndProc);

    while (bThreadRunning)
    {
        if (!currentFb)
        {
            currentFb = ::Graphics::GetBackBuffer();
            continue;
        }

        // Render
        DrawAndSwap();
    }

    Usermode::Input::RemoveMouseHandle(m);
    Usermode::Input::RemoveKeyboardHandle(k);

    sys_exit();
}
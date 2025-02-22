/// -----------
/// windows.cpp
/// @brief This file defines the functions relating to handling window rendering.

#include "windows.hpp"

#include "window/window.hpp"

#include "window_draw_list/window_draw_list.hpp"

#include "../../../kernel/multitasking/syscall.hpp"

namespace Usermode
{
    namespace Graphics
    {
        namespace Windows
        {
            bool bInitialized = false;

            WindowDrawList windowDrawList;
        }
    }
}

extern "C" void printf(const char* f, ...);

void Usermode::Graphics::Windows::Init()
{
    if (bInitialized)
        return;

    windowDrawList.RePlace(sys_malloc(sizeof(void*) * MAX_WINDOWS), MAX_WINDOWS);

    bInitialized = true;
}

static void PutPixel(::Graphics::Framebuffer* fb, int x, int y, uint32_t color){
    if(uint32_t* p = fb->GetPixel(x, y))
        *p = color;
}

static void DrawBox(::Graphics::Framebuffer* fb, int l, int t, int r, int b, uint32_t color){
    for(int y = t; y <= b; y++)
        for(int x = l; x <= r; x++)
            PutPixel(fb, x, y, color);
}

static void RenderWindow(::Graphics::Framebuffer* fb, Window* window){
    DrawBox(fb, window->l, window->t, window->r, window->b, 0x0);
}

void Usermode::Graphics::Windows::RenderWindows(::Graphics::Framebuffer* fb)
{
    for(uint32_t i = 0; i < windowDrawList.GetSize(); i++)
    {
        Window* w = windowDrawList.Get(i);
        if(!w)
        {
            windowDrawList.shouldBeSorted = true;
            continue;
        }

        RenderWindow(fb, w);
    }

    if(windowDrawList.shouldBeSorted)
        windowDrawList.ForceSort();
}
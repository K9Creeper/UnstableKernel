/// -----------
/// windows.cpp
/// @brief This file defines the functions relating to handling window rendering.

#include "windows.hpp"

#include "window_draw_list/window_draw_list.hpp"

#include "../../../kernel/multitasking/syscall.hpp"

#include "../native/style.hpp"

#include "../../../chelpers/memory.h"
#include "../../../chelpers/string.h"

namespace Usermode
{
    namespace Graphics
    {
        extern uint32_t GetScreenX();
        extern uint32_t GetScreenY();

        extern uint32_t GetScreenPitch();
        extern uint32_t GetScreenBPP();

        extern ::Graphics::Framebuffer *currentFb;

        namespace Windows
        {
            bool bInitialized = false;

            WindowDrawList windowDrawList;

            void Init();
            void RenderWindows(::Graphics::Framebuffer *fb);
        }
    }
}

extern "C" void printf(const char *f, ...);

void Usermode::Graphics::Windows::Init()
{
    if (bInitialized)
        return;

    windowDrawList.RePlace(sys_malloc(sizeof(void *) * MAX_WINDOWS), MAX_WINDOWS);

    bInitialized = true;
}

static void PutPixel(::Graphics::Framebuffer *fb, int x, int y, uint32_t color)
{
    if (uint32_t *p = fb->GetPixel(x, y))
        *p = color;
}

static void DrawBox(::Graphics::Framebuffer *fb, int l, int t, int r, int b, uint32_t color)
{
    for (int y = t; y <= b; y++)
        for (int x = l; x <= r; x++)
            PutPixel(fb, x, y, color);
}

static void DrawWindow(::Graphics::Framebuffer *fb, Usermode::Graphics::Windows::Window *window)
{
    ::Graphics::Framebuffer *wfb = &window->framebuffer;
    if (!(window->windowStyleFlags & WS_BORDERLESS || window->windowStyleFlags & WS_FULLSCREEN))
    {
        DrawBox(fb, window->l, window->t, window->r, window->b, Usermode::Graphics::Native::Style::windowAccent);
    }

    // Now work with the wfb
    DrawBox(wfb, 0, 0, window->width, window->height, 0xB00B5);
}

static void RenderWindow(::Graphics::Framebuffer *fb, Usermode::Graphics::Windows::Window *window)
{
    DrawWindow(fb, window);

    // Copy window's fb to current fb
    window->framebuffer.CopyTo(fb, window->viewport.l, window->viewport.t, window->viewport.r, window->viewport.b);
}

Usermode::Graphics::Windows::Window *Usermode::Graphics::Windows::CreateWindow(const char *windowName, uint32_t windowFlags,
                                                                               uint32_t x, uint32_t y,
                                                                               uint32_t width, uint32_t height,
                                                                               bool focus)
{
    Usermode::Graphics::Windows::Window *window = reinterpret_cast<Usermode::Graphics::Windows::Window *>(sys_malloc(sizeof(Usermode::Graphics::Windows::Window)));
    memset(reinterpret_cast<uint8_t *>(window->name), 0, 512);
    memcpy(reinterpret_cast<uint8_t *>(window->name), reinterpret_cast<const uint8_t *>(windowName), strlen(windowName));

    if (!windowFlags)
        window->windowStyleFlags = windowFlags;

    window->width = width;
    window->height = height;

    window->l = x;
    window->t = y;

    window->r = x + width;
    window->b = y + height;

    if (window->windowStyleFlags & WS_BORDERLESS || window->windowStyleFlags & WS_FULLSCREEN)
        window->viewport.t = window->t;
    else
        window->viewport.t = window->t + 50;

    window->viewport.l = window->l;
    window->viewport.r = window->r;

    window->viewport.b = window->b;

    window->framebuffer.Init(sys_malloc(currentFb->GetSize()), Usermode::Graphics::GetScreenX(), Usermode::Graphics::GetScreenY(), Usermode::Graphics::GetScreenPitch(), Usermode::Graphics::GetScreenBPP());
    window->framebuffer.Clear();

    window->framebuffer.used_width = width;
    window->framebuffer.used_height = height;

    Usermode::Graphics::Windows::windowDrawList.AddWindow(window, !focus);

    if (focus)
        Usermode::Graphics::Windows::windowDrawList.FocusWindow(window);

    return window;
}

static bool forceSort = false;

void Usermode::Graphics::Windows::RenderWindows(::Graphics::Framebuffer *fb)
{
    for (uint32_t i = 0; i < windowDrawList.GetSize(); i++)
    {
        Window *w = windowDrawList.Get(i);
        if (!w)
        {
            forceSort = true;
            continue;
        }

        RenderWindow(fb, w);
    }

    if (forceSort)
    {
        windowDrawList.ForceSort();
        forceSort = false;
    }
}
/// -----------
/// windows.cpp
/// @brief This file defines the functions relating to handling window rendering.

#include "windows.hpp"

#include "window_draw_list/window_draw_list.hpp"

#include "../../../kernel/multitasking/syscall.hpp"

#include "../native/style.hpp"

#include "../../../chelpers/memory.h"
#include "../../../chelpers/string.h"

#include "../../input/input.hpp"

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
            void HandleDragFocusInput();
        }

        extern MouseInfo mouseInfoBuffer;
    }
}

void Usermode::Graphics::Windows::Init()
{
    if (bInitialized)
        return;

    windowDrawList.RePlace(sys_malloc(sizeof(void *) * MAX_WINDOWS), MAX_WINDOWS);

    bInitialized = true;
}

static bool PointInRect(int x, int y, int l, int t, int r, int b)
{
    return (x >= l && x <= r) && (y >= t && y <= b);
}

static void PutPixel(::Graphics::Framebuffer *fb, int x, int y, uint32_t color, uint8_t opacity = 255)
{
    uint32_t *pixel = fb->GetPixel(x, y);
    if (pixel)
    {
        if (opacity == 255)
        {
            (*pixel) = color;
        }
        else
        {
            uint8_t srcR = (color >> 16) & 0xFF;
            uint8_t srcG = (color >> 8) & 0xFF;
            uint8_t srcB = color & 0xFF;

            uint8_t dstR = (*pixel >> 16) & 0xFF;
            uint8_t dstG = (*pixel >> 8) & 0xFF;
            uint8_t dstB = (*pixel) & 0xFF;

            uint8_t outR = ((srcR * opacity) + (dstR * (255 - opacity))) / 255;
            uint8_t outG = ((srcG * opacity) + (dstG * (255 - opacity))) / 255;
            uint8_t outB = ((srcB * opacity) + (dstB * (255 - opacity))) / 255;

            *pixel = (outR << 16) | (outG << 8) | outB;
        }
    }
}

static void DrawBox(::Graphics::Framebuffer *fb, int l, int t, int r, int b, uint32_t color, uint8_t opacity = 255)
{
    for (int y = t; y <= b; y++)
        for (int x = l; x <= r; x++)
            PutPixel(fb, x, y, color, opacity);
}

static void DrawBorderBox(::Graphics::Framebuffer *fb, int x, int y, int width, int height, int thickness, uint32_t color)
{
    for (int i = 0; i < thickness; i++)
    {
        for (int j = 0; j < width + (thickness + thickness); j++)
        {
            PutPixel(fb, x + j - thickness, y + i - thickness, color);
            PutPixel(fb, x + j - thickness, y + height + i, color);
        }

        for (int j = 0; j < height + (thickness + thickness); j++)
        {
            PutPixel(fb, x + i - thickness, y + j - thickness, color);
            PutPixel(fb, x + width + i, y + j - thickness, color);
        }
    }
}

static void DrawWindow(::Graphics::Framebuffer *fb, Usermode::Graphics::Windows::Window *window)
{
    ::Graphics::Framebuffer *wfb = &window->framebuffer;
    if (!(window->windowStyleFlags & WS_BORDERLESS || window->windowStyleFlags & WS_FULLSCREEN))
    {
        DrawBox(fb, window->l, window->t, window->r, window->b, Usermode::Graphics::Native::Style::windowAccent, Usermode::Graphics::Native::Style::windowOpacity);
    }

    // Now work with the wfb -- an example
    DrawBox(wfb, 0, 0, window->width, window->height, 0x00FF00);

    if (Usermode::Graphics::Windows::windowDrawList.isWindowFocused(window))
        DrawBorderBox(fb, window->l, window->t, window->width, window->height, Usermode::Graphics::Native::Style::windowBorderThickness, Usermode::Graphics::Native::Style::windowBorderColor);
}

static bool HoveringWindow(Usermode::Graphics::Windows::Window *window)
{
    Usermode::Graphics::Windows::Window *hovering = nullptr;
    for (uint16_t i = 0; i < Usermode::Graphics::Windows::windowDrawList.GetSize(); i++)
    {
        Usermode::Graphics::Windows::Window *w = Usermode::Graphics::Windows::windowDrawList.Get(i);
        if (PointInRect(Usermode::Graphics::mouseInfoBuffer.X, Usermode::Graphics::mouseInfoBuffer.Y, w->l, w->t, w->r, w->b))
        {
            hovering = w;
        }
    }

    return window == hovering;
}

static void RenderWindow(::Graphics::Framebuffer *fb, Usermode::Graphics::Windows::Window *window)
{
    DrawWindow(fb, window);

    // Copy window's fb to current fb
    window->framebuffer.CopyTo(fb, window->viewport.l, window->viewport.t, window->viewport.r, window->viewport.b);
}

extern "C" void printf(const char* f, ...);

Usermode::Graphics::Windows::Window *Usermode::Graphics::Windows::CreateWindow(const char *windowName, uint32_t windowFlags,
                                                                               uint32_t x, uint32_t y,
                                                                               uint32_t width, uint32_t height,
                                                                               void *callback,
                                                                               bool focus)
{
    Usermode::Graphics::Windows::Window *window = reinterpret_cast<Usermode::Graphics::Windows::Window *>(sys_malloc(sizeof(Usermode::Graphics::Windows::Window)));
    memset(reinterpret_cast<uint8_t *>(window->name), 0, 512);
    memcpy(reinterpret_cast<uint8_t *>(window->name), reinterpret_cast<const uint8_t *>(windowName), strlen(windowName));

    window->callback = callback;

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
        window->viewport.t = window->t + 20;

    window->viewport.l = window->l;
    window->viewport.r = window->r;

    window->viewport.b = window->b;

    uint32_t nLfb = sys_malloc(currentFb->GetSize());

    window->framebuffer.Init(nLfb, Usermode::Graphics::GetScreenX(), Usermode::Graphics::GetScreenY(), Usermode::Graphics::GetScreenPitch(), Usermode::Graphics::GetScreenBPP());

    window->framebuffer.used_width = width;
    window->framebuffer.used_height = height;

    Usermode::Graphics::Windows::windowDrawList.AddWindow(window, !focus);

    if (focus)
        Usermode::Graphics::Windows::windowDrawList.FocusWindow(window);

    return window;
}

static bool forceSort = false;

void Usermode::Graphics::Windows::HandleDragFocusInput()
{
    static Window *w;

    static int mDownX;
    static int mDownY;

    // Handle Dragging
    bool mouseDown = Usermode::Graphics::mouseInfoBuffer.currState[0];
    bool mouseWasDown = Usermode::Graphics::mouseInfoBuffer.prevState[0];
    int mouseX = Usermode::Graphics::mouseInfoBuffer.X;
    int mouseY = Usermode::Graphics::mouseInfoBuffer.Y;

    bool isMousePressed = mouseDown && !mouseWasDown;
    bool isMouseReleased = !mouseDown && mouseWasDown;
    bool isMouseHeld = mouseDown && mouseWasDown;

    for (uint32_t i = 0; i < windowDrawList.GetSize(); i++)
    {
        Window *window = windowDrawList.Get(i);

        if (!window)
            continue;

        bool isHovering = HoveringWindow(window);

        if (isHovering)
        {
            if (isMousePressed)
                Usermode::Graphics::Windows::windowDrawList.FocusWindow(window);
        }

        if (isHovering && PointInRect(mouseX, mouseY, window->l, window->t, window->r, window->viewport.t))
        {
            if (isMousePressed)
            {
                w = window;
                mDownX = mouseX;
                mDownY = mouseY;
            }
        }

        if (w == window)
        {
            if (isMouseReleased)
            {
                w = nullptr;
            }
            else if (isMouseHeld)
            {
                int diffX = mouseX - mDownX;
                int diffY = mouseY - mDownY;

                window->l += diffX;
                window->r += diffX;
                window->t += diffY;
                window->b += diffY;

                window->viewport.l += diffX;
                window->viewport.r += diffX;
                window->viewport.t += diffY;
                window->viewport.b += diffY;

                mDownX = mouseX;
                mDownY = mouseY;
            }
        }
    }
}

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
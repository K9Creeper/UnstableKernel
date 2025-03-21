/// ---------------
/// framebuffer.cpp
/// @brief This file defines the functions of the Framebuffer class.

#include "framebuffer.hpp"

#include "../chelpers/memory.h"

extern "C" void printf(const char* f, ...);

void Graphics::Framebuffer::Init(uint32_t *lfb, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp)
{
    printf("x\n");

    this->lfb = lfb;
    this->width = width;
    this->height = height;
    this->pitch = pitch;
    this->bpp = bpp;

    this->used_width = width;
    this->used_height = height;

    this->size = (reinterpret_cast<uint32_t>(lfb) + ((height - 1) * pitch + ((width - 1) * (bpp / 8)))) - reinterpret_cast<uint32_t>(lfb);
    
    printf("d\n");

    Clear();
}

void Graphics::Framebuffer::Clear()
{
    memset(reinterpret_cast<uint8_t *>(lfb), 0, size);
}

uint32_t Graphics::Framebuffer::GetSize() const
{
    return size;
}

uint32_t *Graphics::Framebuffer::GetPixel(int x, int y)
{
    if (y > height - 1 || x > width - 1 || x < 0 || y < 0)
        return nullptr;

    return reinterpret_cast<uint32_t *>(reinterpret_cast<uint32_t>(lfb) + ((y)*pitch + ((x) * (bpp / 8))));
}

uint32_t *Graphics::Framebuffer::GetLFB() const
{
    return lfb;
}

void Graphics::Framebuffer::SetLFB(uint32_t *lfb)
{
    this->lfb = lfb;
}

bool Graphics::Framebuffer::SwapTo(Framebuffer *other)
{
    if (other->GetSize() != size)
        return false;

    memcpy(reinterpret_cast<uint8_t *>(lfb), reinterpret_cast<uint8_t *>(other->GetLFB()), this->size);

    return true;
}

uint32_t Graphics::Framebuffer::GetWidth() const
{
    return width;
}

uint32_t Graphics::Framebuffer::GetHeight() const
{
    return height;
}

uint32_t Graphics::Framebuffer::GetPitch() const
{
    return pitch;
}

uint32_t Graphics::Framebuffer::GetBPP() const
{
    return bpp;
}

void Graphics::Framebuffer::CopyTo(Graphics::Framebuffer *other, int l, int t, int r, int b)
{
    int yy = 0;
    for (int y = t; y < b; ++y)
    {
        int xx = 0;
        for (int x = l; x < r; ++x)
        {
            uint32_t *c = this->GetPixel(xx, yy);
            uint32_t *d = other->GetPixel(x, y);

            ++xx;

            if (!c || !d){
                continue;
            }


            *d = *c;
        }
        ++yy;
    }
}
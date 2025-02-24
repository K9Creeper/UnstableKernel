/// ---------------
/// framebuffer.hpp
/// @brief This defines the Framebuffer class, for handling the system display.
#pragma once

#include <stdint.h>

namespace Graphics
{
    class Framebuffer
    {
    protected:
        uint32_t *lfb;

    private:
        uint32_t width;
        uint32_t height;
        uint32_t pitch;
        uint32_t bpp;

        uint32_t size;

    public:
        uint32_t used_width;
        uint32_t used_height;

        void Init(uint32_t *lfb, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp);

        void Clear();

        uint32_t GetSize() const;

        uint32_t *GetPixel(int x, int y);

        uint32_t *GetLFB() const;
        void SetLFB(uint32_t *lfb);

        // Like copy mem from other to this
        bool SwapTo(Framebuffer *other);

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        uint32_t GetPitch() const;
        uint32_t GetBPP() const;

        // Like copy mem from this to other
        void CopyTo(Graphics::Framebuffer *other, uint32_t l, uint32_t t, uint32_t r, uint32_t b);

    private:
    };
}
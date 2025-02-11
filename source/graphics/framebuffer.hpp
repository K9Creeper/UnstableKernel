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
        void Init(uint32_t *lfb, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp);
        
        void Clear();
        
        uint32_t GetSize()const;

        uint32_t* GetPixel(uint32_t x, uint32_t y);

        uint32_t* GetLFB()const;
        void SetLFB(uint32_t *lfb);

        bool SwapTo(Framebuffer* other);

    private:
    };
}
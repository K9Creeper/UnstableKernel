/// ---------------
/// framebuffer.hpp
/// @brief This defines the Framebuffer class, for handling the system display.

#include <stdint.h>

namespace Graphics
{
    class Framebuffer
    {
    private:
        uint32_t *lfb;

        uint32_t width;
        uint32_t height;
        uint32_t pitch;
        uint32_t bpp;

        uint32_t size;

    public:
        void Init(uint32_t *lfb, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp);
        void Clear();
        
        uint32_t* GetPixel(uint32_t x, uint32_t y);

    private:
    };
}
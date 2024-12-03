#include "graphics.hpp"


namespace Kernel
{
    namespace Graphics
    {
        namespace FrameBuffer
        {
            uint32_t addr = 0;

            uint32_t size = 0;
            
            uint32_t width = 0;
            uint32_t height = 0;

            uint32_t pitch = 0;
            uint8_t bpp = 0;
        }

        bool bInitialized = false;
    }
}

void Kernel::Graphics::Init(uint32_t addr, uint32_t size, uint32_t width, uint32_t height, uint32_t pitch, uint8_t bpp){
    FrameBuffer::addr = addr;

    FrameBuffer::size = size;

    FrameBuffer::width = width;
    FrameBuffer::height = height;

    FrameBuffer::pitch = pitch;
    FrameBuffer::bpp = bpp;

    bInitialized = true;
}

uint32_t Kernel::Graphics::GetPixel(uint32_t x, uint32_t y){
    if (!bInitialized || x >= FrameBuffer::width || y >= FrameBuffer::height) return 0;

    uint32_t offset = y * FrameBuffer::pitch + x * (FrameBuffer::bpp / 8);

    return reinterpret_cast<uint32_t>(FrameBuffer::addr + offset);
}

void Kernel::Graphics::SetPixel(uint32_t x, uint32_t y, uint32_t val){
    if (!bInitialized || x >= FrameBuffer::width || y >= FrameBuffer::height) return;

    uint32_t offset = y * FrameBuffer::pitch + x * (FrameBuffer::bpp / 8);

    *reinterpret_cast<uint32_t*>(FrameBuffer::addr + offset) = val;
}
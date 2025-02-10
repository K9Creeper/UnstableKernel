/// ---------------
/// framebuffer.cpp
/// @brief This file defines the functions of the Framebuffer class.

#include "framebuffer.hpp"

#include "../chelpers/memory.h"

void Graphics::Framebuffer::Init(uint32_t *lfb, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp){
    this->lfb = lfb; 
    this->width = width;
    this->height = height;
    this->pitch = pitch;
    this->bpp = bpp;

    this->size = (reinterpret_cast<uint32_t>(lfb) + ((height-1) * pitch + ((width-1) * (bpp / 8))))-reinterpret_cast<uint32_t>(lfb);
    Clear();
}

void Graphics::Framebuffer::Clear(){
    memset(reinterpret_cast<uint8_t*>(lfb), 0, size);
}

uint32_t* Graphics::Framebuffer::GetPixel(uint32_t x, uint32_t y){
    if(y > height-1 || x > width-1 || x < 0 || y < 0 )
        return nullptr;
        
    return reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(lfb) + ((y) * pitch + ((x) * (bpp / 8))));
}
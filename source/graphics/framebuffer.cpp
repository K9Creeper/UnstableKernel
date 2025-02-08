/// ---------------
/// framebuffer.cpp
/// @brief This file defines the functions of the Framebuffer class.

#include "framebuffer.hpp"

#include "../chelpers/memory.h"

#include "../kernel/memory_management/kheap/kheap.hpp"

void Framebuffer::Set(uint32_t* src, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp){
    this->data = src;
    this->width = width;
    this->height = height;
    this->pitch = pitch;
    this->bpp = bpp;
}

uint32_t* tempData = nullptr;
void Framebuffer::Swap(Framebuffer* other){
    if(!tempData)
        Kernel::MemoryManagement::KHeap::kmalloc_(GetSize());
    
    memcpy(reinterpret_cast<uint8_t*>(tempData), reinterpret_cast<uint8_t*>(other->data), sizeof(GetSize()));
    memcpy(reinterpret_cast<uint8_t*>(other->data), reinterpret_cast<uint8_t*>(data), sizeof(GetSize()));
    memcpy(reinterpret_cast<uint8_t*>(data), reinterpret_cast<uint8_t*>(tempData), sizeof(GetSize()));

    memset(reinterpret_cast<uint8_t*>(tempData), 0, sizeof(GetSize()));
}

void Framebuffer::Clear(){
    memset(reinterpret_cast<uint8_t*>(data), 0, GetSize());
}

void Framebuffer::PutPixel(uint32_t x, uint32_t y, uint32_t color){
    uint32_t* pixel = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(data) + ((y) * pitch + ((x) * (bpp / 8))));
    if(pixel)
        *pixel = color;
}

uint32_t Framebuffer::GetSize()const{
    return (reinterpret_cast<uint32_t>(data) + ((height-1) * pitch + ((width-1) * (bpp / 8)))) - reinterpret_cast<uint32_t>(data); 
}
/// ------------
/// framebuffer.hpp
/// @brief This file defines the functions of the Framebuffer class.

#include "framebuffer.hpp"

template <typename type_t>
Framebuffer<type_t>::Framebuffer() {}

template <typename type_t>
uint32_t Framebuffer<type_t>:: GetDataLocation(){
    return reinterpret_cast<uint32_t>(data);
}

template <typename type_t>
void Framebuffer<type_t>::RePlace(uint32_t address)
{
    this->data = address;
}

template <typename type_t>
type_t &Framebuffer<type_t>::operator[](uint32_t i){
    return data[i];
}

template <typename type_t>
type_t &Framebuffer<type_t>::Get(uint32_t i){
    return data[i];
}

template <typename type_t>
void Framebuffer<type_t>::Swap(Framebuffer<type_t> &other)
{
    if (width != other.width || height != other.height || pitch != other.pitch || bpp != other.bpp)
        return;

    type_t temp;
    for (uint32_t p = 0; p < width * height * (bpp / 8); p++)
    {
        temp = data[p];
        data[p] = other[p];
        other[p] = temp;
    }
}
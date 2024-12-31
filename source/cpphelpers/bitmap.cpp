/// -----------
/// bitmap.cpp
/// @brief This file defines the core functions of the template based Bitmap class.

#include "bitmap.hpp"

#include "../chelpers/memory.h"

template <typename type_t>
void Bitmap<type_t>::RePlace(void *addr, uint32_t memSize)
{
    this->array = reinterpret_cast<type_t *>(addr);
    SetMemSize(memSize);
}

template <typename type_t>
type_t &Bitmap<type_t>::operator[](uint32_t i)
{
    return this->array[i];
}

template <typename type_t>
type_t& Bitmap<type_t>::Get(uint32_t i)
{
    return this->array[i];
}

template <typename type_t>
void Bitmap<type_t>::Set(uint32_t i, type_t val)
{
    this->array[i] = val;
}

template <typename type_t>
void Bitmap<type_t>::SetCount(uint32_t count)
{
    this->count = count;
    this->memSize = count / bytePerEntry;
}

template <typename type_t>
void Bitmap<type_t>::SetMemSize(uint32_t memSize)
{
    this->memSize = memSize;
    this->count = memSize * bytePerEntry;
}

template <typename type_t>
uint32_t Bitmap<type_t>::GetCount() const
{
    return this->count;
}

template <typename type_t>
uint32_t Bitmap<type_t>::GetMemSize() const
{
    return this->memSize;
}

template <typename type_t>
uint32_t Bitmap<type_t>::GetTypeSize() const
{
    return sizeof_type_t;
}

template <typename type_t>
uint32_t Bitmap<type_t>::GetBytesPerEntry() const
{
    return bytePerEntry;
}
template <typename type_t>
uint32_t Bitmap<type_t>::IndexFromBit(uint32_t i) const{
    return i / bytePerEntry;
}

template <typename type_t>

uint32_t Bitmap<type_t>::OffsetFromBit(uint32_t i) const{
    return i % bytePerEntry;
}

template <typename type_t>
void Bitmap<type_t>::Clear()
{
    memset(reinterpret_cast<unsigned char *>(array), 0, count * sizeof(type_t));
}

template class Bitmap<uint32_t>;
#pragma once

#include <stdint.h>

template <typename type_t>
class Bitmap
{
protected:
    type_t *array;

    uint32_t size;
    uint32_t memSize;

    uint32_t sizeof_type_t;
public:
    Bitmap() : array{nullptr}, size{0}, sizeof_type_t{sizeof(type_t)} {}
    ~Bitmap();

    void RePlace(void *addr, uint32_t size = 0);

    void SetSize(uint32_t size);
    void SetMemSize(uint32_t memcSize);

    type_t& operator[](uint32_t i);

    type_t Get(uint32_t i);

    void Set(uint32_t i, type_t val);

    uint32_t GetSize()const;
    uint32_t GetMemSize()const;

    uint32_t GetTypeSize() const;

    virtual void Clear();
};

template class Bitmap<uint32_t>;
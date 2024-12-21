/// -----------
/// bitmap.hpp
/// @brief This file declares the template based Bitmap class.

#pragma once

#include <stdint.h>

template <typename type_t>
class Bitmap
{
protected:
    type_t *array;

    uint32_t count;
    uint32_t memSize;
    
    uint32_t sizeof_type_t;
    uint32_t bytePerEntry;
    
public:
    Bitmap() {}
    ~Bitmap() {}

    void Create(){
        array = nullptr;
        count = 0;
        memSize = 0;
    
        sizeof_type_t = sizeof(type_t);
        bytePerEntry = sizeof_type_t * 8;
    }

    void RePlace(void *addr, uint32_t memSize = 0);

    void SetCount(uint32_t count);
    void SetMemSize(uint32_t memSize);

    type_t& operator[](uint32_t i);

    type_t& Get(uint32_t i);

    void Set(uint32_t i, type_t val);

    uint32_t GetCount()const;
    uint32_t GetMemSize()const;

    uint32_t GetTypeSize() const;
    uint32_t GetBytesPerEntry() const;

    uint32_t IndexFromBit(uint32_t i)const;
    uint32_t OffsetFromBit(uint32_t i)const;

    virtual void Clear();
};

template class Bitmap<uint32_t>;
#include "bitmap.hpp"

#include "../chelpers/memory.h"

template <typename type_t>
Bitmap<type_t>::~Bitmap(){ }

template <typename type_t>
void Bitmap<type_t>::RePlace(void *addr, uint32_t size){
    this->array = reinterpret_cast<type_t*>(addr);
    this->size = size;
}

template <typename type_t>
type_t& Bitmap<type_t>::operator[](uint32_t i){
    return this->array[i];
}

template <typename type_t>
type_t Bitmap<type_t>::Get(uint32_t i){
    return this->array[i];
}

template <typename type_t>
void Bitmap<type_t>::Set(uint32_t i, type_t val){
    this->array[i] = val;
}

template <typename type_t>
void Bitmap<type_t>::SetSize(uint32_t size){
    this->size = size;
    this->memSize = size * sizeof(type_t);
}

template <typename type_t>
void Bitmap<type_t>::SetMemSize(uint32_t memSize){
    this->memSize = memSize;
    this->size = memSize / sizeof(type_t);
}

template <typename type_t>
uint32_t Bitmap<type_t>::GetSize()const{
    return this->size;
}

template <typename type_t>
uint32_t Bitmap<type_t>::GetMemSize()const{
    return this->memSize;
}

template <typename type_t>
uint32_t Bitmap<type_t>::GetTypeSize() const{
    return sizeof_type_t;
}

template <typename type_t>
void Bitmap<type_t>::Clear(){
    memset(reinterpret_cast<unsigned char*>(array), 0, size * sizeof(type_t));
}
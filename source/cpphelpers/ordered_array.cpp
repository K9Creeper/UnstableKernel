/// -----------------
/// ordered_array.cpp
/// @brief This file defines core functions of the template based OrderedArray class.

#include "ordered_array.hpp"

#include "../chelpers/memory.h"

template <typename type_t>
OrderedArray<type_t>::~OrderedArray() {}

template <typename type_t>
void OrderedArray<type_t>::RePlace(void *address, uint32_t max_size)
{
	array = reinterpret_cast<type_t*>(address);

	memset(reinterpret_cast<unsigned char *>(address), 0, max_size * sizeof(type_t));
	
	size = 0;
	this->max_size = max_size;
}

template <typename type_t>
type_t OrderedArray<type_t>::Get(uint32_t i)
{
	if(i >= this->max_size)
		return this->array[this->max_size - 1];
	return this->array[i];
}

template <typename type_t>
void OrderedArray<type_t>::Remove(uint32_t i) {
    while (i < this->size - 1) {
        this->array[i] = this->array[i + 1];
        i++;
    }
    this->size--;
}

template <typename type_t>
uint32_t OrderedArray<type_t>::GetSize() const
{
	return size;
}
template <typename type_t>
uint32_t OrderedArray<type_t>::GetMaxSize() const
{
	return max_size;
}

template class OrderedArray<void*>;
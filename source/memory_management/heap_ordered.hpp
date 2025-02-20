/// -----------------
/// heap_ordered.hpp
/// @brief This file declares the HeapOrderedArray used for the kheap in the kernel.

#pragma once

#include "../cpphelpers/ordered_array.hpp"

class HeapOrderedArray : public OrderedArray<void *> {
    public:
        bool Insert(void* item);

    private:
        bool lessthan_(void *a, void *b);
};
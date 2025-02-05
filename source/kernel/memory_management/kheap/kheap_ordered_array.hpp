/// -----------------
/// kheap_ordered.hpp
/// @brief This file declares the KHeapOrderedArray used for the kheap in the kernel.

#pragma once

#include "../../../cpphelpers/ordered_array.hpp"

class KHeapOrderedArray : public OrderedArray<void *> {
  bool lessthan_(void *a, void *b);
};
#pragma once

#include "../../../cpphelpers/ordered_array.hpp"

template class OrderedArray<void*>;

class KHeapOrderedArray : public OrderedArray<void *> {
  bool lessthan_(void *a, void *b);
};
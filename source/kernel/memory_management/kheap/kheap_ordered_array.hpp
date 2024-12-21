#pragma once

#include "../../../cpphelpers/ordered_array.hpp"

class KHeapOrderedArray : public OrderedArray<void *> {
  bool lessthan_(void *a, void *b);
};

template class OrderedArray<void*>;
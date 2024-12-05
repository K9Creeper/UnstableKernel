#pragma once

#include "../../../cpphelpers/ordered_array.hpp"

class HeapIndexOrderedArray : public OrderedArray<void *> {
  bool HeapIndexOrderedArray::lessthan_(void *a, void *b);
};
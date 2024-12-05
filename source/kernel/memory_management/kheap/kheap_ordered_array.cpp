#include "kheap_ordered_array.hpp"

#include "kheap.hpp"

bool KHeapOrderedArray::lessthan_(void *a, void *b)
{
  return (((Kernel::MemoryManagement::KHeap::Header *)a)->size < ((Kernel::MemoryManagement::KHeap::Header *)b)->size);
}
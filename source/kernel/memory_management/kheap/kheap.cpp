#include "kheap.hpp"

#define KHEAP_MAX_END 0xCFFFF000
#define KHEAP_INITIAL_SIZE 0x100000 // arbitrary
#define MAX_HEAP_SIZE 0xCFFFF000
#define HEAP_INDEX_SIZE 0x20000 // arbitrary
#define HEAP_MAGIC 0xDEADBEEF   // unusual number that will stand out from others
#define HEAP_MIN_SIZE 0x70000   // arbitrary

#include "../../memory/memory.hpp"

#include "../paging/paging.hpp"

#include "kheap_ordered_map.hpp"

namespace Kernel
{
  namespace MemoryManagement
  {
    namespace KHeap
    {
      bool bInitialized = false;

      namespace Early
      {
        uint32_t nextPlacementAddress = 0;
      }

    }
  }
}



HeapIndexOrderedArray heapIndex;
uint32_t heapMax;
bool heapSupervisor;
bool heapReadOnly;

void Kernel::MemoryManagement::KHeap::Early::PreInit(uint32_t kernel_end)
{
  nextPlacementAddress = kernel_end;
}
uint32_t Kernel::MemoryManagement::KHeap::Early::pkmalloc_(uint32_t size, bool shouldAlign, uint32_t *physAddress)
{
  if (!Kernel::MemoryManagement::KHeap::bInitialized)
  {
    // if the heap is not initialized, manually handle allocation using placement address

    if (shouldAlign && (nextPlacementAddress & 0x00000FFF))
    {
      // #define PAGE_ALIGN_MASK 0xFFFFF000
      nextPlacementAddress &= 0xFFFFF000; // Align the address down
      nextPlacementAddress += 0x1000;     // Move to the next page boundary
    }

    // the current placement address
    uint32_t base = nextPlacementAddress;

    // the physical address is requested, return the base address
    if (physAddress != nullptr)
      *physAddress = base;

    // update the placement address due to the newly the yoinked space
    nextPlacementAddress += size;

    // return the base address of the allocation
    return base;
  }
  return 0;
}

void Kernel::MemoryManagement::KHeap::Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly)
{
  heapIndex.RePlace(reinterpret_cast<void *>(start), HEAP_INDEX_SIZE);
  start += sizeof(void *) * HEAP_INDEX_SIZE;

  if ((start & 0xFFFFF000) != 0)
  {
    start &= 0xFFFFF000;
    start += 0x1000;
  }

  Memory::Info::kheap_start = start;
  Memory::Info::kheap_end = end;

  heapMax = max;
  heapSupervisor = supervisor;
  heapReadOnly = readonly;

  Header *hole = reinterpret_cast<Header *>(start);
  hole->size = end - start;
  hole->magic = HEAP_MAGIC;
  hole->is_hole = 1;

  heapIndex.Insert(reinterpret_cast<void *>(hole));
}

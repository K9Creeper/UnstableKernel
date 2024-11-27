#include "kheap.hpp"

#include "../../../c_helpers/memory.h"

#include "../paging/paging.hpp"

/// Take a look here: https://github.com/levex/osdev/blob/master/memory/malloc.c

extern uint32_t linkerld_endofkernel;

extern "C" ordered_array_t orderedArray_create(uint32_t max_size, lessthan_predicate_t less_than);
extern "C" ordered_array_t orderedArray_place(void *addr, uint32_t max_size, lessthan_predicate_t less_than);
extern "C" void orderedArray_destroy(ordered_array_t *array);
extern "C" void orderedArray_insert(type_t item, ordered_array_t *array);
extern "C" type_t orderedArray_lookup(uint32_t i, ordered_array_t *array);
extern "C" void orderedArray_remove(uint32_t i, ordered_array_t *array);

namespace Kernel
{
  namespace Memory
  {
    namespace KHeap
    {
      bool bInitialized = false;

      uint32_t placementAddress = reinterpret_cast<uint32_t>(&linkerld_endofkernel);

      Heap *heap = nullptr;
    }
  }
}

void *Alloc(uint32_t size, uint8_t page_align);

extern "C" uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t *physAddress)
{
  if (Kernel::Memory::KHeap::bInitialized)
  {
    void *address = Alloc(size, align);

    if (physAddress != 0)
    {
      Kernel::Memory::Paging::PageEntry *page = Kernel::Memory::Paging::GetPageEntry(reinterpret_cast<uint32_t>(address), Kernel::Memory::Paging::kernelDirectory);

      *physAddress = page->frame * PAGE_SIZE + (reinterpret_cast<uint32_t>(address) & 0xFFF);
    }

    return reinterpret_cast<uint32_t>(address);
  }
  else
  {
    if (align == 1 && ((Kernel::Memory::KHeap::placementAddress & PAGE_ALIGN_MASK) != 0))
    {
      Kernel::Memory::KHeap::placementAddress &= PAGE_ALIGN_MASK;
      Kernel::Memory::KHeap::placementAddress += PAGE_SIZE;
    }

    uint32_t base = Kernel::Memory::KHeap::placementAddress;

    if (physAddress)
      *physAddress = base;

    Kernel::Memory::KHeap::placementAddress += size;

    return base;
  }
}

static int FindSmallestHole(uint32_t size, int8_t page_align)
{
  uint32_t i = 0;

  while (i < Kernel::Memory::KHeap::heap->index.size)
  {
    Kernel::Memory::KHeap::Header *header = reinterpret_cast<Kernel::Memory::KHeap::Header *>(orderedArray_lookup(i, &Kernel::Memory::KHeap::heap->index));

    if (page_align > 0)
    {
      uint32_t location = reinterpret_cast<uint32_t>(header);
      uint32_t hole_start = location + sizeof(Kernel::Memory::KHeap::Header);
      int offset = 0;

      if ((hole_start & PAGE_ALIGN_MASK) != 0)
      {
        offset = PAGE_SIZE - (hole_start % PAGE_SIZE); // ? why mod
      }

      int hole_size = static_cast<int>(header->size) - offset;

      if (hole_size >= static_cast<int>(size))
      {
        break;
      }
    }
    else if (header->size >= size)
    {
      break;
    }

    i += 1;
  }

  if (i == Kernel::Memory::KHeap::heap->index.size)
  {
    return -1; // didn't find anything
  }
  else
  {
    return i;
  }
}

static void Expand(uint32_t new_size)
{
  uint32_t i;

  // Get the nearest following page boundary
  if ((new_size & PAGE_ALIGN_MASK) != 0)
  {
    new_size &= PAGE_ALIGN_MASK;
    new_size += PAGE_SIZE;
  }

  // This should always be on a page boundary
  uint32_t old_size = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;

  // ?
  i = old_size;

  while (i < new_size)
  {
    // ?
    Kernel::Memory::Paging::AllocateFrame(
        Kernel::Memory::Paging::MakePageEntry(Kernel::Memory::KHeap::heap->startAddress + i, Kernel::Memory::Paging::kernelDirectory),
        Kernel::Memory::KHeap::heap->supervisor ? 1 : 0,
        Kernel::Memory::KHeap::heap->readonly ? 0 : 1);

    i += PAGE_SIZE;
  }

  //
  Kernel::Memory::KHeap::heap->endAddress = Kernel::Memory::KHeap::heap->startAddress + new_size;
}

static uint32_t Contract(uint32_t new_size)
{
  uint32_t i;

  // Get the nearest following page boundary
  if (new_size & PAGE_SIZE) // why is this check different ??
  {
    new_size &= PAGE_SIZE;
    new_size += PAGE_SIZE;
  }

  // Make sure we are not overreaching
  if (new_size < HEAP_MIN_SIZE)
  {
    new_size = HEAP_MIN_SIZE;
  }

  // This should always be on a page boundary
  uint32_t old_size = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;

  // ?
  i = old_size - PAGE_SIZE; // why is this different ??

  while (new_size < i)
  {
    // ?
    Kernel::Memory::Paging::FreeFrame(
        Kernel::Memory::Paging::GetPageEntry(Kernel::Memory::KHeap::heap->startAddress + i, Kernel::Memory::Paging::kernelDirectory));

    i -= PAGE_SIZE;
  }

  //
  Kernel::Memory::KHeap::heap->endAddress = Kernel::Memory::KHeap::heap->startAddress + new_size;

  return new_size;
}

void *Alloc(uint32_t size, uint8_t page_align)
{
  // Make sure we take the size of header/footer into account
  uint32_t new_size = size + sizeof(Kernel::Memory::KHeap::Header) + sizeof(Kernel::Memory::KHeap::Footer);

  // Find smallest hole that wil fit
  int i = FindSmallestHole(new_size, page_align);

  // Didn't find a suitable hole
  if (i == -1)
  {
    // Save some previous data
    uint32_t old_length = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;
    uint32_t old_end_address = Kernel::Memory::KHeap::heap->endAddress;

    // We need to allocate more space
    Expand(old_length + new_size);

    uint32_t new_length = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;

    // Find the endmost header
    i = 0;

    uint32_t idx = -1;
    uint32_t value = 0;

    while (i < Kernel::Memory::KHeap::heap->index.size)
    {
      uint32_t tmp = reinterpret_cast<uint32_t>(orderedArray_lookup(i, &Kernel::Memory::KHeap::heap->index));

      if (tmp > value)
      {
        value = tmp;
        idx = i;
      }

      i += 1;
    }

    // If we didn't find any headers, we need to add one
    if (idx == -1)
    {
      //
      Kernel::Memory::KHeap::Header *header = (Kernel::Memory::KHeap::Header *)old_end_address;

      header->size = new_length - old_length;
      header->magic = HEAP_MAGIC;
      header->is_hole = 1;

      //
      Kernel::Memory::KHeap::Footer *footer = (Kernel::Memory::KHeap::Footer *)(old_end_address + header->size - sizeof(Kernel::Memory::KHeap::Footer));

      footer->header = header;
      footer->magic = HEAP_MAGIC;

      //
      orderedArray_insert((void *)header, &Kernel::Memory::KHeap::heap->index);
    }

    //
    else
    {
      // The last header needs adjusting
      Kernel::Memory::KHeap::Header *header = reinterpret_cast<Kernel::Memory::KHeap::Header *>(orderedArray_lookup(idx, &Kernel::Memory::KHeap::heap->index));

      header->size += new_length - old_length;

      // Rewrite the footer
      Kernel::Memory::KHeap::Footer *footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)header + header->size - sizeof(Kernel::Memory::KHeap::Footer));

      footer->header = header;
      footer->magic = HEAP_MAGIC;
    }

    // We now have enough space. Recurse, and call the function again
    return Alloc(size, page_align);
  }

  // Get header pointer from index given to us by find_smallest_hole ??
  Kernel::Memory::KHeap::Header *orig_hole_header = reinterpret_cast<Kernel::Memory::KHeap::Header *>(orderedArray_lookup(i, &Kernel::Memory::KHeap::heap->index));
  uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
  uint32_t orig_hole_size = orig_hole_header->size;

  // If the leftover is less than the overhead for adding a new hole,
  uint32_t leftover = orig_hole_size - new_size;
  if (leftover < sizeof(Kernel::Memory::KHeap::Header) + sizeof(Kernel::Memory::KHeap::Footer))
  {
    // Then just increase the requested size to the size of the hole we found
    size += leftover;

    new_size = orig_hole_size;
  }

  // If we need to page-align the data, do it now and make a new hole in front of our block
  if (page_align && ((orig_hole_pos & PAGE_ALIGN_MASK) != 0))
  {
    uint32_t size2, footer_pos;

    //
    size2 = PAGE_SIZE - (orig_hole_pos & 0xFFF) - sizeof(Kernel::Memory::KHeap::Header); // ??

    //
    Kernel::Memory::KHeap::Header *hole_header = (Kernel::Memory::KHeap::Header *)orig_hole_pos;

    hole_header->size = size2; // ??
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;

    //
    footer_pos = orig_hole_pos + size2 - sizeof(Kernel::Memory::KHeap::Footer); // ??

    //
    Kernel::Memory::KHeap::Footer *hole_footer = (Kernel::Memory::KHeap::Footer *)(footer_pos);

    hole_footer->header = hole_header;
    hole_footer->magic = HEAP_MAGIC;

    //
    orig_hole_pos += size2;
    orig_hole_size -= size2;
  }

  // Else don't need this hole any more, delete it from index
  else
  {
    orderedArray_remove(i, &Kernel::Memory::KHeap::heap->index);
  }

  // Overwrite original header and footer
  Kernel::Memory::KHeap::Header *block_header = (Kernel::Memory::KHeap::Header *)orig_hole_pos;

  block_header->size = new_size;
  block_header->magic = HEAP_MAGIC;
  block_header->is_hole = 0;

  Kernel::Memory::KHeap::Footer *block_footer = (Kernel::Memory::KHeap::Footer *)(orig_hole_pos + sizeof(Kernel::Memory::KHeap::Header) + size);

  block_footer->header = block_header;
  block_footer->magic = HEAP_MAGIC;

  // We may need to write a new hole after the allocated block.
  // We do this only if the new hole would have positive size?
  if (orig_hole_size - new_size > 0)
  {
    Kernel::Memory::KHeap::Header *hole_header = (Kernel::Memory::KHeap::Header *)(orig_hole_pos + sizeof(Kernel::Memory::KHeap::Header) + size + sizeof(Kernel::Memory::KHeap::Footer));

    hole_header->size = orig_hole_size - new_size;
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;

    Kernel::Memory::KHeap::Footer *hole_footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)hole_header + orig_hole_size - new_size - sizeof(Kernel::Memory::KHeap::Footer)); // ?

    if ((uint32_t)hole_footer < Kernel::Memory::KHeap::heap->endAddress)
    {
      hole_footer->header = hole_header;
      hole_footer->magic = HEAP_MAGIC;
    }
    orderedArray_insert((void *)hole_header, &Kernel::Memory::KHeap::heap->index);
  }

  //
  return (void *)((uint32_t)block_header + sizeof(Kernel::Memory::KHeap::Header));
}

extern "C" void free_(void *p)
{
  uint32_t i;

  // Exit gracefully for null pointers
  if (p == 0)
  {
    return;
  }

  // Get the header and footer associated with the pointer
  Kernel::Memory::KHeap::Header *header = (Kernel::Memory::KHeap::Header *)((uint32_t)p - sizeof(Kernel::Memory::KHeap::Header));
  Kernel::Memory::KHeap::Footer *footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)header + header->size - sizeof(Kernel::Memory::KHeap::Footer));

  //

  // Make us a hole
  header->is_hole = 1;

  // Do we want to add this header into the 'free-holes' index
  char do_add = 1;

  // Unify left
  // If the thing immediately to the left of us is a footer...
  Kernel::Memory::KHeap::Footer *test_footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)header - sizeof(Kernel::Memory::KHeap::Footer));

  if (test_footer->magic == HEAP_MAGIC &&
      test_footer->header->is_hole == 1)
  {
    uint32_t cache_size = header->size; // Cache our current size

    header = test_footer->header; // Rewrite our header with the new one
    footer->header = header;      // Rewrite our footer to point to the new header

    header->size += cache_size; // Change the header size

    do_add = 0; // Since this header is already in the index, we don't want to add it again
  }

  // Unify right
  // If the thing immediately to the right of us is a header...

  Kernel::Memory::KHeap::Header *test_header = (Kernel::Memory::KHeap::Header *)((uint32_t)footer + sizeof(Kernel::Memory::KHeap::Footer));

  if (test_header->magic == HEAP_MAGIC && test_header->is_hole)
  {
    // Increase our size
    header->size += test_header->size;

    // Rewrite it's footer to point to our header
    test_footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)test_header + test_header->size - sizeof(Kernel::Memory::KHeap::Footer));
    footer = test_footer;

    // Find and remove this header from the index
    i = 0;
    while ((i < Kernel::Memory::KHeap::heap->index.size) &&
           (reinterpret_cast<void *>(orderedArray_lookup(i, &Kernel::Memory::KHeap::heap->index)) != (void *)test_header))
    {
      i += 1;
    }

    // Remove it
    orderedArray_remove(i, &Kernel::Memory::KHeap::heap->index);
  }

  // If the footer location is the end address, we can contract
  if ((uint32_t)footer + sizeof(Kernel::Memory::KHeap::Footer) == Kernel::Memory::KHeap::heap->endAddress)
  {
    uint32_t old_length = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;

    uint32_t new_length = Contract((uint32_t)header - Kernel::Memory::KHeap::heap->startAddress);

    // Check how big we will be after resizing

    // We will still exist, so resize us
    if (header->size - (old_length - new_length) > 0)
    {
      header->size -= old_length - new_length;

      footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)header + header->size - sizeof(Kernel::Memory::KHeap::Footer));

      footer->header = header;
      footer->magic = HEAP_MAGIC;
    }
    // We will no longer exist. Remove from index
    else
    {
      //
      while ((i < Kernel::Memory::KHeap::heap->index.size) &&
             (reinterpret_cast<void *>(orderedArray_lookup(i, &Kernel::Memory::KHeap::heap->index)) != (void *)test_header))
      {
        i += 1;
      }

      // If we didn't find ourselves, we have nothing to remove
      if (i < Kernel::Memory::KHeap::heap->index.size)
      {
        orderedArray_remove(i, &Kernel::Memory::KHeap::heap->index);
      }
    }
  }

  //
  if (do_add == 1)
  {
    orderedArray_insert((void *)header, &Kernel::Memory::KHeap::heap->index);
  }
}

extern "C" char header_t_less_than(void *a, void *b)
{
  return ((Kernel::Memory::KHeap::Header *)a)->size < ((Kernel::Memory::KHeap::Header *)b)->size ? 1 : 0;
}

void Kernel::Memory::KHeap::Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly)
{

  Kernel::Memory::KHeap::heap = reinterpret_cast<Kernel::Memory::KHeap::Heap *>(kmalloc_(sizeof(Kernel::Memory::KHeap::Heap), 0, 0));
  Kernel::Memory::KHeap::heap->index = orderedArray_place((void *)start, HEAP_INDEX_SIZE, &header_t_less_than);

  start += sizeof(void *) * HEAP_INDEX_SIZE;

  if ((start & PAGE_ALIGN_MASK) != 0)
  {
    start &= PAGE_ALIGN_MASK;
    start += PAGE_SIZE;
  }

  Kernel::Memory::KHeap::heap->startAddress = start;
  Kernel::Memory::KHeap::heap->endAddress = end;
  Kernel::Memory::KHeap::heap->maxAddress = max;
  Kernel::Memory::KHeap::heap->supervisor = supervisor;
  Kernel::Memory::KHeap::heap->readonly = readonly;

  Kernel::Memory::KHeap::Header *hole = reinterpret_cast<Kernel::Memory::KHeap::Header *>(start);

  hole->size = end - start;
  hole->magic = HEAP_MAGIC;
  hole->is_hole = 1;

  orderedArray_insert((void *)hole, &Kernel::Memory::KHeap::heap->index);

  Kernel::Memory::KHeap::bInitialized = true;
}
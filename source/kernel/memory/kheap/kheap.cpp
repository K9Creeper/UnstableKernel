#include "kheap.hpp"

#include "../../../chelpers/memory.h"
#include "../../../chelpers/ordered_array.h"

#include "../paging/paging.hpp"

extern uint32_t linkerld_endofkernel;

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

static int FindSmallestHole(uint32_t size, int8_t page_align)
{
  uint32_t i = 0;
  for (; i < Kernel::Memory::KHeap::heap->index.size; i++)
  {
    // get the header of the current block in the ordered array
    Kernel::Memory::KHeap::Header *header = reinterpret_cast<Kernel::Memory::KHeap::Header *>(
        orderedArray_lookup(i, &Kernel::Memory::KHeap::heap->index));

    // check if we need to align
    if (page_align > 0)
    {
      uint32_t location = reinterpret_cast<uint32_t>(header);
      uint32_t hole_start = location + sizeof(Kernel::Memory::KHeap::Header);
      uint32_t offset = 0;

      // calculate the offset to align
      if ((location + sizeof(Kernel::Memory::KHeap::Header) & 0xFFFFF000) != 0)
      {
        offset = PAGE_SIZE - (hole_start % PAGE_SIZE);
      }

      // calculate the available hole size after alignment
      uint32_t hole_size = header->size - offset;

      // if the hole size is sufficient for the requested size, stop the search
      if (hole_size >= size)
        break;
    }
    else if (header->size >= size) // if no alignment is needed, just check if the block is large enough
      break;
  }

  // return the index if found
  if (i == Kernel::Memory::KHeap::heap->index.size)
    return -1; // no suitable block

  return i; // found a suitable block
}

static void Expand(uint32_t new_size)
{
  // get the nearest following page boundary
  if ((new_size & PAGE_ALIGN_MASK) != 0)
  {
    new_size &= PAGE_ALIGN_MASK;
    new_size += PAGE_SIZE;
  }

  // this should always be on a page boundary
  uint32_t old_size = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;

  for (uint32_t i = old_size; i < new_size; i += PAGE_SIZE)
  {
    // allocate a frame at the current address
    Kernel::Memory::Paging::AllocateFrame(
        Kernel::Memory::Paging::MakePageEntry(
            Kernel::Memory::KHeap::heap->startAddress + i,
            Kernel::Memory::Paging::kernelDirectory),
        Kernel::Memory::KHeap::heap->supervisor ? 1 : 0,
        Kernel::Memory::KHeap::heap->readonly ? 0 : 1);
  }

  // update the heap end address
  Kernel::Memory::KHeap::heap->endAddress = Kernel::Memory::KHeap::heap->startAddress + new_size;
}

uint32_t Contract(uint32_t new_size)
{
  // get the nearest following page boundary
  if (new_size & PAGE_SIZE)
  {
    new_size &= PAGE_SIZE;
    new_size += PAGE_SIZE;
  }

  // make sure we are not overreaching
  if (new_size < HEAP_MIN_SIZE)
  {
    new_size = HEAP_MIN_SIZE;
  }

  // this should always be on a page boundary
  uint32_t old_size = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;

  for (uint32_t i = old_size - PAGE_SIZE; new_size < i; i -= PAGE_SIZE)
  {
    // free the page
    Kernel::Memory::Paging::FreeFrame(
        Kernel::Memory::Paging::GetPageEntry(
            Kernel::Memory::KHeap::heap->startAddress + i,
            Kernel::Memory::Paging::kernelDirectory));
  }

  // update the heap end address
  Kernel::Memory::KHeap::heap->endAddress = Kernel::Memory::KHeap::heap->startAddress + new_size;

  return new_size;
}

void *Alloc(uint32_t size, uint8_t page_align)
{
  // account for the size of header and footer
  uint32_t new_size = size + sizeof(Kernel::Memory::KHeap::Header) + sizeof(Kernel::Memory::KHeap::Footer);

  // the smallest hole that can fit the new size
  int i = FindSmallestHole(new_size, page_align);

  // if no suitable hole is found
  if (i == -1)
  {
    // save previous data to expand the heap
    uint32_t old_length = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;
    uint32_t old_end_address = Kernel::Memory::KHeap::heap->endAddress;

    // allocate more space for the heap
    Expand(old_length + new_size);

    uint32_t new_length = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;

    i = 0;
    uint32_t idx = -1;
    uint32_t value = 0;

    // find the last header in the heap
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

    // if no headers found, create a new one at the end of the heap
    if (idx == -1)
    {
      Kernel::Memory::KHeap::Header *header = (Kernel::Memory::KHeap::Header *)old_end_address;
      header->size = new_length - old_length;
      header->magic = HEAP_MAGIC;
      header->is_hole = 1;

      // create the footer
      Kernel::Memory::KHeap::Footer *footer = (Kernel::Memory::KHeap::Footer *)(old_end_address + header->size - sizeof(Kernel::Memory::KHeap::Footer));
      footer->header = header;
      footer->magic = HEAP_MAGIC;

      // insert the new header/footer pair
      orderedArray_insert((void *)header, &Kernel::Memory::KHeap::heap->index);
    }
    else
    {
      // if found
      // adjust the last found header to include the newly allocated space
      Kernel::Memory::KHeap::Header *header = reinterpret_cast<Kernel::Memory::KHeap::Header *>(orderedArray_lookup(idx, &Kernel::Memory::KHeap::heap->index));
      header->size += new_length - old_length;

      // update the footer to match the new header size
      Kernel::Memory::KHeap::Footer *footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)header + header->size - sizeof(Kernel::Memory::KHeap::Footer));
      footer->header = header;
      footer->magic = HEAP_MAGIC;
    }

    // recursively call the allocation function again
    // with the expanded heap
    return Alloc(size, page_align);
  }

  // Retrieve the original header of the found hole
  Kernel::Memory::KHeap::Header *orig_hole_header = reinterpret_cast<Kernel::Memory::KHeap::Header *>(orderedArray_lookup(i, &Kernel::Memory::KHeap::heap->index));
  uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
  uint32_t orig_hole_size = orig_hole_header->size;

  // If the leftover space is too small to create a new hole, increase the allocated size to fill the hole
  uint32_t leftover = orig_hole_size - new_size;
  if (leftover < sizeof(Kernel::Memory::KHeap::Header) + sizeof(Kernel::Memory::KHeap::Footer))
  {
    size += leftover;
    new_size = orig_hole_size;
  }

  // If page alignment is requested, adjust the position and create a new hole
  if (page_align && ((orig_hole_pos & PAGE_ALIGN_MASK) != 0))
  {
    uint32_t size2, footer_pos;

    // Calculate the size needed to align the hole
    size2 = PAGE_SIZE - (orig_hole_pos & 0xFFF) - sizeof(Kernel::Memory::KHeap::Header);

    // Create a new header for the aligned hole
    Kernel::Memory::KHeap::Header *hole_header = (Kernel::Memory::KHeap::Header *)orig_hole_pos;
    hole_header->size = size2;
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;

    // Calculate the footer position and create the footer
    footer_pos = orig_hole_pos + size2 - sizeof(Kernel::Memory::KHeap::Footer);
    Kernel::Memory::KHeap::Footer *hole_footer = (Kernel::Memory::KHeap::Footer *)(footer_pos);
    hole_footer->header = hole_header;
    hole_footer->magic = HEAP_MAGIC;

    // Adjust the original hole position and size
    orig_hole_pos += size2;
    orig_hole_size -= size2;
  }
  else
  {
    // If no page alignment is needed, remove the hole from the index
    orderedArray_remove(i, &Kernel::Memory::KHeap::heap->index);
  }

  // Overwrite the original hole header and footer with the new block allocation
  Kernel::Memory::KHeap::Header *block_header = (Kernel::Memory::KHeap::Header *)orig_hole_pos;
  block_header->size = new_size;
  block_header->magic = HEAP_MAGIC;
  block_header->is_hole = 0;

  Kernel::Memory::KHeap::Footer *block_footer = (Kernel::Memory::KHeap::Footer *)(orig_hole_pos + sizeof(Kernel::Memory::KHeap::Header) + size);
  block_footer->header = block_header;
  block_footer->magic = HEAP_MAGIC;

  // If there is leftover space after the allocated block, create a new hole
  if (orig_hole_size - new_size > 0)
  {
    Kernel::Memory::KHeap::Header *hole_header = (Kernel::Memory::KHeap::Header *)(orig_hole_pos + sizeof(Kernel::Memory::KHeap::Header) + size + sizeof(Kernel::Memory::KHeap::Footer));
    hole_header->size = orig_hole_size - new_size;
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;

    Kernel::Memory::KHeap::Footer *hole_footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)hole_header + orig_hole_size - new_size - sizeof(Kernel::Memory::KHeap::Footer));

    // If the footer is within the heap's end address, update it
    if ((uint32_t)hole_footer < Kernel::Memory::KHeap::heap->endAddress)
    {
      hole_footer->header = hole_header;
      hole_footer->magic = HEAP_MAGIC;
    }

    // Insert the new hole into the ordered array
    orderedArray_insert((void *)hole_header, &Kernel::Memory::KHeap::heap->index);
  }

  // Return the allocated memory address, skipping the header
  return (void *)((uint32_t)block_header + sizeof(Kernel::Memory::KHeap::Header));
}

void Free(void *p)
{
  uint32_t i;

  // null pointers = mean
  if (p == 0)
  {
    return;
  }

  // get the header and footer
  Kernel::Memory::KHeap::Header *header = (Kernel::Memory::KHeap::Header *)((uint32_t)p - sizeof(Kernel::Memory::KHeap::Header));
  Kernel::Memory::KHeap::Footer *footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)header + header->size - sizeof(Kernel::Memory::KHeap::Footer));

  // make a hole
  header->is_hole = 1;

  // add header into the 'free-holes' index
  char do_add = 1;

  // unify left if the left is a footer
  Kernel::Memory::KHeap::Footer *test_footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)header - sizeof(Kernel::Memory::KHeap::Footer));

  if (test_footer->magic == HEAP_MAGIC && test_footer->header->is_hole == 1)
  {
    uint32_t cache_size = header->size;

    header = test_footer->header;
    footer->header = header;

    header->size += cache_size;

    do_add = 0;
  }

  // unify right if the right of is a header
  Kernel::Memory::KHeap::Header *test_header = (Kernel::Memory::KHeap::Header *)((uint32_t)footer + sizeof(Kernel::Memory::KHeap::Footer));

  if (test_header->magic == HEAP_MAGIC && test_header->is_hole)
  {
    // increase our size
    header->size += test_header->size;

    // rewrite footer to point to our header
    test_footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)test_header + test_header->size - sizeof(Kernel::Memory::KHeap::Footer));
    footer = test_footer;

    // find and remove header from the index
    i = 0;
    while ((i < Kernel::Memory::KHeap::heap->index.size) &&
           (reinterpret_cast<void *>(orderedArray_lookup(i, &Kernel::Memory::KHeap::heap->index)) != (void *)test_header))
    {
      i += 1;
    }

    // bye bye it
    orderedArray_remove(i, &Kernel::Memory::KHeap::heap->index);
  }

  // if the footer location is the end address, we can contract
  if ((uint32_t)footer + sizeof(Kernel::Memory::KHeap::Footer) == Kernel::Memory::KHeap::heap->endAddress)
  {
    uint32_t old_length = Kernel::Memory::KHeap::heap->endAddress - Kernel::Memory::KHeap::heap->startAddress;

    uint32_t new_length = Contract((uint32_t)header - Kernel::Memory::KHeap::heap->startAddress);

    // ned to check how big after resizing

    // if it will still exist, so it
    if (header->size - (old_length - new_length) > 0)
    {
      header->size -= old_length - new_length;

      footer = (Kernel::Memory::KHeap::Footer *)((uint32_t)header + header->size - sizeof(Kernel::Memory::KHeap::Footer));

      footer->header = header;
      footer->magic = HEAP_MAGIC;
    }

    else
    {
      // if it will not exist. bye bye it

      // search for it
      while ((i < Kernel::Memory::KHeap::heap->index.size) && (reinterpret_cast<void *>(orderedArray_lookup(i, &Kernel::Memory::KHeap::heap->index)) != (void *)test_header))
      {
        i += 1;
      }

      // if no find, no remove
      if (i < Kernel::Memory::KHeap::heap->index.size)
      {
        orderedArray_remove(i, &Kernel::Memory::KHeap::heap->index);
      }
    }
  }

  // erm we still want to add it to the index, so do it
  if (do_add == 1)
  {
    orderedArray_insert((void *)header, &Kernel::Memory::KHeap::heap->index);
  }
}

uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t *physAddress)
{
  if (Kernel::Memory::KHeap::bInitialized || Kernel::Memory::KHeap::heap != 0 )
  {
    // if the kernel heap is initialized, allocate memory
    void *address = Alloc(size, align);

    // if physical address is requested, calculate and return it
    if (physAddress != nullptr)
    {
      // retrieve the page entry
      Kernel::Memory::Paging::PageEntry *page = Kernel::Memory::Paging::GetPageEntry(
          reinterpret_cast<uint32_t>(address), Kernel::Memory::Paging::kernelDirectory);

      // calculate the physical address based on the page frame and the offset
      // and manipulate the addr
      *physAddress = page->frame * PAGE_SIZE + (reinterpret_cast<uint32_t>(address) & 0xFFF);
    }

    // return the virtual address of the newly allocated memory
    return reinterpret_cast<uint32_t>(address);
  }
  else
  {
    // if the heap is not initialized, manually handle allocation using placement address

    // if requested page-align
    if (align == 1 && (Kernel::Memory::KHeap::placementAddress & 0x00000FFF))
    {
      // #define PAGE_ALIGN_MASK 0xFFFFF000
      Kernel::Memory::KHeap::placementAddress &= PAGE_ALIGN_MASK; // Align the address down
      Kernel::Memory::KHeap::placementAddress += PAGE_SIZE;       // Move to the next page boundary
    }

    // the current placement address
    uint32_t base = Kernel::Memory::KHeap::placementAddress;

    // the physical address is requested, return the base address
    if (physAddress != nullptr)
    {
      *physAddress = base;
    }

    // update the placement address due to the newly the yoinked space
    Kernel::Memory::KHeap::placementAddress += size;

    // return the base address of the allocation
    return base;
  }
}

void kfree_(void* ptr){
  Free(ptr);
}

extern "C" char header_t_less_than(void *a, void *b)
{
  // return true if 'a' is smaller ('b' is larger).
  return ((Kernel::Memory::KHeap::Header *)a)->size < ((Kernel::Memory::KHeap::Header *)b)->size ? 1 : 0;
}
extern "C" void printf(const char* format, ...);
void Kernel::Memory::KHeap::Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly)
{
  // allocate heap
  Kernel::Memory::KHeap::heap = reinterpret_cast<Kernel::Memory::KHeap::Heap *>(kmalloc_(sizeof(Kernel::Memory::KHeap::Heap), 0, 0));
  // make the ordered_array start at the address provided by start
  Kernel::Memory::KHeap::heap->index = orderedArray_place((void *)start, HEAP_INDEX_SIZE, &header_t_less_than);
  // add space to compinsate
  start += sizeof(void *) * HEAP_INDEX_SIZE;
  // make sure it is aligned and stuff
  if ((start & PAGE_ALIGN_MASK) != 0)
  {
    start &= PAGE_ALIGN_MASK;
    start += PAGE_SIZE;
  }
  // put in our funny thangs
  Kernel::Memory::KHeap::heap->startAddress = start;
  Kernel::Memory::KHeap::heap->endAddress = end;
  Kernel::Memory::KHeap::heap->maxAddress = max;
  Kernel::Memory::KHeap::heap->supervisor = supervisor;
  Kernel::Memory::KHeap::heap->readonly = readonly;

  // make a hole at the start of the heap
  Kernel::Memory::KHeap::Header *hole = reinterpret_cast<Kernel::Memory::KHeap::Header *>(start);
  hole->size = end - start;
  hole->magic = HEAP_MAGIC;
  hole->is_hole = 1;

  // insert the hole to the index.
  orderedArray_insert((void *)hole, &Kernel::Memory::KHeap::heap->index);
  // oh thats right, this is a init function, and we reached the end.
  Kernel::Memory::KHeap::bInitialized = true;
}
/// ---------
/// heap.cpp
/// @brief This file defines the functions and structures to handle and allow for dynamic memory allocation in the system.

#include "heap.hpp"

#include "../chelpers/memory.h"

#include "paging.hpp"

#define HEAP_MAGIC 0xDEADBEEF
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MIN_SIZE 0x70000

extern "C" void printf(const char* f, ...);

bool HeapOrderedArray::Insert(void *item)
{
  uint32_t i;

  static void *tmp;
  static void *tmp2;

  i = 0;

  while (i < this->size && this->lessthan_(this->array[i], item))
  {
    i++;
  }

  if (i + 1 > this->max_size)
    return false;

  // If reached end of array, just append the item
  if (i == this->size)
  {
    this->array[this->size] = item;

    this->size++;
  }
  else // insert
  {
    tmp = this->array[i];

    this->array[i] = item;

    while (i < this->size)
    {
      i++;

      tmp2 = this->array[i];

      this->array[i] = tmp;

      tmp = tmp2;
    }

    this->size++;
  }

  return true;
}

bool HeapOrderedArray::lessthan_(void *a, void *b)
{
  return ((reinterpret_cast<Header *>(a))->size < (reinterpret_cast<Header *>(b))->size);
}

uint32_t Heap::GetHeapStart() const
{
  return heap_start;
}

uint32_t Heap::GetHeapEnd() const
{
  return heap_end;
}

uint32_t Heap::GetHeapMax() const
{
  return heap_max_address;
}

void Heap::PreInit(uint32_t end)
{
  if (bPreInitialized)
    return;

    nextPlacementAddress = end;

    bPreInitialized = true;
}

void Heap::Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly, Paging *pMan)
{
  if (bInitialized || !bPreInitialized || !pMan)
    return;

  pManager = pMan;

  pManager->AllocateRegion(start, end, false, !supervisor, !readonly);

  bSupervisor = supervisor;
  bReadOnly = readonly;

  heapIndex.RePlace(reinterpret_cast<void *>(start), HEAP_INDEX_SIZE);
  
  start += sizeof(void *) * HEAP_INDEX_SIZE;

  if ((start & 0xFFFFF000) != 0)
  {
    start &= 0xFFFFF000;
    start += 0x1000;
  }

  heap_start = start;
  heap_end = end;
  heap_max_address = max;

  Header *hole = reinterpret_cast<Header *>(start);
  hole->size = end - start;
  hole->magic = HEAP_MAGIC;
  hole->is_hole = true;

  heapIndex.Insert(reinterpret_cast<void *>(hole));

  bInitialized = true;
}

int Heap::FindSmallestHole(uint32_t size, bool page_align)
{
  uint32_t i = 0;
  for (; i < heapIndex.GetSize(); i++)
  {
    // get the header of the current block in the ordered array
    Header *header = reinterpret_cast<Header *>(heapIndex.Get(i));

    // check if we need to align
    if (page_align > 0)
    {
      uint32_t location = reinterpret_cast<uint32_t>(header);
      uint32_t hole_start = location + sizeof(Header);
      uint32_t offset = 0;

      // calculate the offset to align
      if ((location + sizeof(Header) & 0xFFFFF000) != 0)
      {
        offset = 0x1000 - (hole_start % 0x1000);
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
  if (i == heapIndex.GetSize())
    return -1; // no suitable block

  return i; // found a suitable block
}

void Heap::Expand(uint32_t new_size)
{
  // get the nearest following page boundary
  if ((new_size & 0xFFFFF000) != 0)
  {
    new_size &= 0xFFFFF000;
    new_size += 0x1000;
  }

  for (uint32_t i = 0; i < new_size + 0x1000; i += 0x1000)
  {
    // allocate a frame at the current address
    pManager->AllocatePage(heap_end + i, 0, !bSupervisor, !bReadOnly);
  }

  // update the heap end address
  heap_end = heap_start + new_size;  
}

uint32_t Heap::Contract(uint32_t new_size)
{
  // get the nearest following page boundary
  if (new_size & 0x1000)
  {
    new_size &= 0x1000;
    new_size += 0x1000;
  }

  // make sure we are not overreaching
  if (new_size < HEAP_MIN_SIZE)
  {
    new_size = HEAP_MIN_SIZE;
  }

  // this should always be on a page boundary
  uint32_t old_size = heap_end - heap_start;

  for (uint32_t i = old_size - 0x1000; new_size < i; i -= 0x1000)
  {
    // Free page
    pManager->FreePage(i, true);
  }

  // update the heap end address
  heap_end = heap_start + new_size;

  return new_size;
}

void *Heap::Alloc(uint32_t size, bool page_align)
{
  // account for the size of header and footer
  uint32_t new_size = size + sizeof(Header) + sizeof(Footer);

  // the smallest hole that can fit the new size
  int i = FindSmallestHole(new_size, page_align);

  // if no suitable hole is found
  if (i == -1)
  {
    // save previous data to expand the heap
    uint32_t old_length = heap_end - heap_start;
    uint32_t old_end_address = heap_end;

    // allocate more space for the heap
    Expand(old_length + new_size);

    uint32_t new_length = heap_end - heap_start;

    i = 0;
    uint32_t idx = -1;
    uint32_t value = 0;

    // find the last header in the heap
    while (i < heapIndex.GetSize())
    {
      uint32_t tmp = reinterpret_cast<uint32_t>(heapIndex.Get(i));

      if (tmp > value)
      {
        value = tmp;
        idx = i;
      }

      i++;
    }

    // if no headers found, create a new one at the end of the heap
    if (idx == -1)
    {
      Header *header = (Header *)old_end_address;
      header->size = new_length - old_length;
      header->magic = HEAP_MAGIC;
      header->is_hole = 1;

      // create the footer
      Footer *footer = (Footer *)(old_end_address + header->size - sizeof(Footer));
      footer->header = header;
      footer->magic = HEAP_MAGIC;

      // insert the new header/footer pair
      heapIndex.Insert((void *)header);
    }
    else
    {
      // if found
      // adjust the last found header to include the newly allocated space
      Header *header = reinterpret_cast<Header *>(heapIndex.Get(idx));
      header->size += new_length - old_length;

      // update the footer to match the new header size
      Footer *footer = (Footer *)((uint32_t)header + header->size - sizeof(Footer));
      footer->header = header;
      footer->magic = HEAP_MAGIC;
    }

    // recursively call the allocation function again
    // with the expanded heap
    return Alloc(size, page_align);
  }

  // Retrieve the original header of the found hole
  Header *orig_hole_header = reinterpret_cast<Header *>(heapIndex.Get(i));
  uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
  uint32_t orig_hole_size = orig_hole_header->size;

  // If the leftover space is too small to create a new hole, increase the allocated size to fill the hole
  uint32_t leftover = orig_hole_size - new_size;
  if (leftover < sizeof(Header) + sizeof(Footer))
  {
    size += leftover;
    new_size = orig_hole_size;
  }

  // If page alignment is requested, adjust the position and create a new hole
  if (page_align && ((orig_hole_pos & 0xFFFFF000) != 0))
  {
    uint32_t size2, footer_pos;

    // Calculate the size needed to align the hole
    size2 = 0x1000 - (orig_hole_pos & 0xFFF) - sizeof(Header);

    // Create a new header for the aligned hole
    Header *hole_header = (Header *)orig_hole_pos;
    hole_header->size = size2;
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;

    // Calculate the footer position and create the footer
    footer_pos = orig_hole_pos + size2 - sizeof(Footer);
    Footer *hole_footer = (Footer *)(footer_pos);
    hole_footer->header = hole_header;
    hole_footer->magic = HEAP_MAGIC;

    // Adjust the original hole position and size
    orig_hole_pos += size2;
    orig_hole_size -= size2;
  }
  else
  {
    // If no page alignment is needed, remove the hole from the index
    heapIndex.Remove(i);
  }

  // Overwrite the original hole header and footer with the new block allocation
  Header *block_header = (Header *)orig_hole_pos;
  block_header->size = new_size;
  block_header->magic = HEAP_MAGIC;
  block_header->is_hole = 0;

  Footer *block_footer = (Footer *)(orig_hole_pos + sizeof(Header) + size);
  block_footer->header = block_header;
  block_footer->magic = HEAP_MAGIC;

  // If there is leftover space after the allocated block, create a new hole
  if (orig_hole_size - new_size > 0)
  {
    Header *hole_header = (Header *)(orig_hole_pos + sizeof(Header) + size + sizeof(Footer));
    hole_header->size = orig_hole_size - new_size;
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;

    Footer *hole_footer = (Footer *)((uint32_t)hole_header + orig_hole_size - new_size - sizeof(Footer));

    // If the footer is within the heap's end address, update it
    if ((uint32_t)hole_footer < heap_end)
    {
      hole_footer->header = hole_header;
      hole_footer->magic = HEAP_MAGIC;
    }

    // Insert the new hole into the ordered array
    heapIndex.Insert((void *)hole_header);
  }

  // Return the allocated memory address, skipping the header
  return (void *)((uint32_t)block_header + sizeof(Header));
}

void Heap::Free(void *p)
{
  uint32_t i;

  // null pointers = mean
  if (p == 0)
  {
    return;
  }

  // get the header and footer
  Header *header = (Header *)((uint32_t)p - sizeof(Header));
  Footer *footer = (Footer *)((uint32_t)header + header->size - sizeof(Footer));

  // make a hole
  header->is_hole = 1;

  // add header into the 'free-holes' index
  char do_add = 1;

  // unify left if the left is a footer
  Footer *test_footer = (Footer *)((uint32_t)header - sizeof(Footer));

  if (test_footer->magic == HEAP_MAGIC && test_footer->header->is_hole == 1)
  {
    uint32_t cache_size = header->size;

    header = test_footer->header;
    footer->header = header;

    header->size += cache_size;

    do_add = 0;
  }

  // unify right if the right of is a header
  Header *test_header = (Header *)((uint32_t)footer + sizeof(Footer));

  if (test_header->magic == HEAP_MAGIC && test_header->is_hole)
  {
    // increase our size
    header->size += test_header->size;

    // rewrite footer to point to our header
    test_footer = (Footer *)((uint32_t)test_header + test_header->size - sizeof(Footer));
    footer = test_footer;

    // find and remove header from the index
    i = 0;
    while ((i < heapIndex.GetSize()) &&
           (reinterpret_cast<void *>(heapIndex.Get(i)) != (void *)test_header))
    {
      i += 1;
    }

    // bye bye it
    heapIndex.Remove(i);
  }

  // if the footer location is the end address, we can contract
  if ((uint32_t)footer + sizeof(Footer) == heap_end)
  {
    uint32_t old_length = heap_end - heap_start;

    uint32_t new_length = Contract((uint32_t)header - heap_start);

    // ned to check how big after resizing

    // if it will still exist, so it
    if (header->size - (old_length - new_length) > 0)
    {
      header->size -= old_length - new_length;

      footer = (Footer *)((uint32_t)header + header->size - sizeof(Footer));

      footer->header = header;
      footer->magic = HEAP_MAGIC;
    }

    else
    {
      // if it will not exist. bye bye it

      // search for it
      while ((i < heapIndex.GetSize()) && (reinterpret_cast<void *>(heapIndex.Get(i)) != (void *)test_header))
      {
        i += 1;
      }

      // if no find, no remove
      if (i < heapIndex.GetSize())
      {
        heapIndex.Remove(i);
      }
    }
  }

  // erm we still want to add it to the index, so do it
  if (do_add == 1)
  {
    heapIndex.Insert((void *)header);
  }
}

uint32_t Heap::malloc_(uint32_t size, bool shouldAlign, uint32_t *physAddress)
{
  if (bInitialized)
  {
    // if the kernel heap is initialized, allocate memory
    uint32_t address = Alloc(size, shouldAlign);

    // if physical address is requested, calculate and return it
    if (physAddress != nullptr)
    {
      *physAddress = pManager->Virtual2Physical(address);
    }

    // return the virtual address of the newly allocated memory
    return address;
  }
  else if (bPreInitialized)
  {
    if (shouldAlign && (nextPlacementAddress & 0x00000FFF))
    {
      nextPlacementAddress &= 0xFFFFF000; // align down
      nextPlacementAddress += 0x1000;     // move to the next page boundary
    }

    uint32_t base = nextPlacementAddress;

    if (physAddress != nullptr)
      *physAddress = base;

    // update the placement address due to the newly the yoinked space
    nextPlacementAddress += size;

    // return the base address of the allocation
    return base;
  }

  return 0x0;
}

void Heap::free(uint32_t address)
{
  if (bInitialized)
  {
    Free(reinterpret_cast<void *>(address));
  }
}
#include "kheap.hpp"

#include "../../memory/memory.hpp"

#include "../paging/paging.hpp"

#include "kheap_ordered_array.hpp"

bool KHeapOrderedArray::lessthan_(void *a, void *b)
{
  return (((Kernel::MemoryManagement::KHeap::Header *)a)->size < ((Kernel::MemoryManagement::KHeap::Header *)b)->size);
}

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

KHeapOrderedArray heapIndex;
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

static int FindSmallestHole(uint32_t size, bool page_align)
{
  uint32_t i = 0;
  for (; i < heapIndex.GetSize(); i++)
  {
    // get the header of the current block in the ordered array
    Kernel::MemoryManagement::KHeap::Header *header = reinterpret_cast<Kernel::MemoryManagement::KHeap::Header *>(heapIndex.Get(i));

    // check if we need to align
    if (page_align > 0)
    {
      uint32_t location = reinterpret_cast<uint32_t>(header);
      uint32_t hole_start = location + sizeof(Kernel::MemoryManagement::KHeap::Header);
      uint32_t offset = 0;

      // calculate the offset to align
      if ((location + sizeof(Kernel::MemoryManagement::KHeap::Header) & 0xFFFFF000) != 0)
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

static void Expand(uint32_t new_size)
{
  // get the nearest following page boundary
  if ((new_size & 0xFFFFF000) != 0)
  {
    new_size &= 0xFFFFF000;
    new_size += 0x1000;
  }

  // this should always be on a page boundary
  uint32_t old_size = Kernel::Memory::Info::kheap_end - Kernel::Memory::Info::kheap_start;

  for (uint32_t i = old_size; i < new_size; i += 0x1000)
  {
    // allocate a frame at the current address
    Kernel::MemoryManagement::Paging::AllocateFrame(
        Kernel::MemoryManagement::Paging::MakePageEntry(
            Kernel::Memory::Info::kheap_start + i,
            Kernel::MemoryManagement::Paging::kernelDirectory),
        heapSupervisor ? 1 : 0,
        heapReadOnly ? 0 : 1);
  }

  // update the heap end address
  Kernel::Memory::Info::kheap_end = Kernel::Memory::Info::kheap_start + new_size;
}

uint32_t Contract(uint32_t new_size)
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
  uint32_t old_size = Kernel::Memory::Info::kheap_end - Kernel::Memory::Info::kheap_start;

  for (uint32_t i = old_size - 0x1000; new_size < i; i -= 0x1000)
  {
    // free the page
    Kernel::MemoryManagement::Paging::FreeFrame(
        Kernel::MemoryManagement::Paging::GetPageEntry(
            Kernel::Memory::Info::kheap_start + i,
            Kernel::MemoryManagement::Paging::kernelDirectory));
  }

  // update the heap end address
  Kernel::Memory::Info::kheap_end = Kernel::Memory::Info::kheap_start + new_size;

  return new_size;
}

void *Alloc(uint32_t size, bool page_align)
{
  // account for the size of header and footer
  uint32_t new_size = size + sizeof(Kernel::MemoryManagement::KHeap::Header) + sizeof(Kernel::MemoryManagement::KHeap::Footer);

  // the smallest hole that can fit the new size
  int i = FindSmallestHole(new_size, page_align);

  // if no suitable hole is found
  if (i == -1)
  {
    // save previous data to expand the heap
    uint32_t old_length = Kernel::Memory::Info::kheap_end - Kernel::Memory::Info::kheap_start;
    uint32_t old_end_address = Kernel::Memory::Info::kheap_end;

    // allocate more space for the heap
    Expand(old_length + new_size);

    uint32_t new_length = Kernel::Memory::Info::kheap_end - Kernel::Memory::Info::kheap_start;

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

      i += 1;
    }

    // if no headers found, create a new one at the end of the heap
    if (idx == -1)
    {
      Kernel::MemoryManagement::KHeap::Header *header = (Kernel::MemoryManagement::KHeap::Header *)old_end_address;
      header->size = new_length - old_length;
      header->magic = HEAP_MAGIC;
      header->is_hole = 1;

      // create the footer
      Kernel::MemoryManagement::KHeap::Footer *footer = (Kernel::MemoryManagement::KHeap::Footer *)(old_end_address + header->size - sizeof(Kernel::MemoryManagement::KHeap::Footer));
      footer->header = header;
      footer->magic = HEAP_MAGIC;

      // insert the new header/footer pair
      heapIndex.Insert((void *)header);
    }
    else
    {
      // if found
      // adjust the last found header to include the newly allocated space
      Kernel::MemoryManagement::KHeap::Header *header = reinterpret_cast<Kernel::MemoryManagement::KHeap::Header *>(heapIndex.Get(idx));
      header->size += new_length - old_length;

      // update the footer to match the new header size
      Kernel::MemoryManagement::KHeap::Footer *footer = (Kernel::MemoryManagement::KHeap::Footer *)((uint32_t)header + header->size - sizeof(Kernel::MemoryManagement::KHeap::Footer));
      footer->header = header;
      footer->magic = HEAP_MAGIC;
    }

    // recursively call the allocation function again
    // with the expanded heap
    return Alloc(size, page_align);
  }

  // Retrieve the original header of the found hole
  Kernel::MemoryManagement::KHeap::Header *orig_hole_header = reinterpret_cast<Kernel::MemoryManagement::KHeap::Header *>(heapIndex.Get(i));
  uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
  uint32_t orig_hole_size = orig_hole_header->size;

  // If the leftover space is too small to create a new hole, increase the allocated size to fill the hole
  uint32_t leftover = orig_hole_size - new_size;
  if (leftover < sizeof(Kernel::MemoryManagement::KHeap::Header) + sizeof(Kernel::MemoryManagement::KHeap::Footer))
  {
    size += leftover;
    new_size = orig_hole_size;
  }

  // If page alignment is requested, adjust the position and create a new hole
  if (page_align && ((orig_hole_pos & 0xFFFFF000) != 0))
  {
    uint32_t size2, footer_pos;

    // Calculate the size needed to align the hole
    size2 = 0x1000 - (orig_hole_pos & 0xFFF) - sizeof(Kernel::MemoryManagement::KHeap::Header);

    // Create a new header for the aligned hole
    Kernel::MemoryManagement::KHeap::Header *hole_header = (Kernel::MemoryManagement::KHeap::Header *)orig_hole_pos;
    hole_header->size = size2;
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;

    // Calculate the footer position and create the footer
    footer_pos = orig_hole_pos + size2 - sizeof(Kernel::MemoryManagement::KHeap::Footer);
    Kernel::MemoryManagement::KHeap::Footer *hole_footer = (Kernel::MemoryManagement::KHeap::Footer *)(footer_pos);
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
  Kernel::MemoryManagement::KHeap::Header *block_header = (Kernel::MemoryManagement::KHeap::Header *)orig_hole_pos;
  block_header->size = new_size;
  block_header->magic = HEAP_MAGIC;
  block_header->is_hole = 0;

  Kernel::MemoryManagement::KHeap::Footer *block_footer = (Kernel::MemoryManagement::KHeap::Footer *)(orig_hole_pos + sizeof(Kernel::MemoryManagement::KHeap::Header) + size);
  block_footer->header = block_header;
  block_footer->magic = HEAP_MAGIC;

  // If there is leftover space after the allocated block, create a new hole
  if (orig_hole_size - new_size > 0)
  {
    Kernel::MemoryManagement::KHeap::Header *hole_header = (Kernel::MemoryManagement::KHeap::Header *)(orig_hole_pos + sizeof(Kernel::MemoryManagement::KHeap::Header) + size + sizeof(Kernel::MemoryManagement::KHeap::Footer));
    hole_header->size = orig_hole_size - new_size;
    hole_header->magic = HEAP_MAGIC;
    hole_header->is_hole = 1;

    Kernel::MemoryManagement::KHeap::Footer *hole_footer = (Kernel::MemoryManagement::KHeap::Footer *)((uint32_t)hole_header + orig_hole_size - new_size - sizeof(Kernel::MemoryManagement::KHeap::Footer));

    // If the footer is within the heap's end address, update it
    if ((uint32_t)hole_footer < Kernel::Memory::Info::kheap_end)
    {
      hole_footer->header = hole_header;
      hole_footer->magic = HEAP_MAGIC;
    }

    // Insert the new hole into the ordered array
    heapIndex.Insert((void *)hole_header);
  }

  // Return the allocated memory address, skipping the header
  return (void *)((uint32_t)block_header + sizeof(Kernel::MemoryManagement::KHeap::Header));
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
  Kernel::MemoryManagement::KHeap::Header *header = (Kernel::MemoryManagement::KHeap::Header *)((uint32_t)p - sizeof(Kernel::MemoryManagement::KHeap::Header));
  Kernel::MemoryManagement::KHeap::Footer *footer = (Kernel::MemoryManagement::KHeap::Footer *)((uint32_t)header + header->size - sizeof(Kernel::MemoryManagement::KHeap::Footer));

  // make a hole
  header->is_hole = 1;

  // add header into the 'free-holes' index
  char do_add = 1;

  // unify left if the left is a footer
  Kernel::MemoryManagement::KHeap::Footer *test_footer = (Kernel::MemoryManagement::KHeap::Footer *)((uint32_t)header - sizeof(Kernel::MemoryManagement::KHeap::Footer));

  if (test_footer->magic == HEAP_MAGIC && test_footer->header->is_hole == 1)
  {
    uint32_t cache_size = header->size;

    header = test_footer->header;
    footer->header = header;

    header->size += cache_size;

    do_add = 0;
  }

  // unify right if the right of is a header
  Kernel::MemoryManagement::KHeap::Header *test_header = (Kernel::MemoryManagement::KHeap::Header *)((uint32_t)footer + sizeof(Kernel::MemoryManagement::KHeap::Footer));

  if (test_header->magic == HEAP_MAGIC && test_header->is_hole)
  {
    // increase our size
    header->size += test_header->size;

    // rewrite footer to point to our header
    test_footer = (Kernel::MemoryManagement::KHeap::Footer *)((uint32_t)test_header + test_header->size - sizeof(Kernel::MemoryManagement::KHeap::Footer));
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
  if ((uint32_t)footer + sizeof(Kernel::MemoryManagement::KHeap::Footer) == Kernel::Memory::Info::kheap_end)
  {
    uint32_t old_length = Kernel::Memory::Info::kheap_end - Kernel::Memory::Info::kheap_start;

    uint32_t new_length = Contract((uint32_t)header - Kernel::Memory::Info::kheap_start);

    // ned to check how big after resizing

    // if it will still exist, so it
    if (header->size - (old_length - new_length) > 0)
    {
      header->size -= old_length - new_length;

      footer = (Kernel::MemoryManagement::KHeap::Footer *)((uint32_t)header + header->size - sizeof(Kernel::MemoryManagement::KHeap::Footer));

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

uint32_t Kernel::MemoryManagement::KHeap::kmalloc_(uint32_t size, bool align, uint32_t *physAddress)
{
  if (Kernel::MemoryManagement::KHeap::bInitialized)
  {
    // if the kernel heap is initialized, allocate memory
    void *address = Alloc(size, align);

    // if physical address is requested, calculate and return it
    if (physAddress != nullptr)
    {
      Kernel::MemoryManagement::Paging::Page *page = Kernel::MemoryManagement::Paging::GetPageEntry(
          reinterpret_cast<uint32_t>(address), Kernel::MemoryManagement::Paging::kernelDirectory);

      // calculate the physical address based on the page frame and the offset
      // and manipulate the addr
      *physAddress = page->frame * 0x1000 + (reinterpret_cast<uint32_t>(address) & 0xFFF);
    }

    // return the virtual address of the newly allocated memory
    return reinterpret_cast<uint32_t>(address);
  }
  else
    return Kernel::MemoryManagement::KHeap::Early::pkmalloc_(size, align, physAddress);
}

void kfree_(void *ptr)
{
  Free(ptr);
}

extern "C" void printf(const char *format, ...);
void Kernel::MemoryManagement::KHeap::Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly)
{
  // allocate heap
  // make the ordered_array start at the address provided by start
  heapIndex.RePlace((void *)start, HEAP_INDEX_SIZE);
  // add space to compinsate
  start += sizeof(void *) * HEAP_INDEX_SIZE;
  // make sure it is aligned and stuff
  if ((start & 0xFFFFF000) != 0)
  {
    start &= 0xFFFFF000;
    start += 0x1000;
  }
  // put in our funny thangs
  Kernel::Memory::Info::kheap_start = start;
  Kernel::Memory::Info::kheap_end = end;
  Kernel::Memory::Info::kheap_max_address = max;

  heapSupervisor = supervisor;
  heapReadOnly = readonly;

  // make a hole at the start of the heap
  Kernel::MemoryManagement::KHeap::Header *hole = reinterpret_cast<Kernel::MemoryManagement::KHeap::Header *>(start);
  hole->size = end - start;
  hole->magic = HEAP_MAGIC;
  hole->is_hole = 1;

  // insert the hole to the index.
  heapIndex.Insert((void *)hole);
  // oh thats right, this is a init function, and we reached the end.
  Kernel::MemoryManagement::KHeap::bInitialized = true;
}
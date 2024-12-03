#include "paging.hpp"

#include "../kheap/kheap.hpp"

#include "../../../c_helpers/memory.h"

#define INDEX_FROM_BIT(a) (a / 32)
#define OFFSET_FROM_BIT(a) (a % 32)

namespace Kernel
{
	namespace Memory
	{
		namespace Paging
		{
			PageDirectory *kernelDirectory = nullptr;
			PageDirectory *currentDirectory = nullptr;

			Frames frames;
		}
	}
}

Kernel::Memory::Paging::PageEntry *Kernel::Memory::Paging::GetPageEntry(uint32_t address, Kernel::Memory::Paging::PageDirectory *dir, bool sMake, uint32_t *out_physical_address)
{
	// turn the address into an index
	uint32_t frame = address / PAGE_SIZE;

	// Find the page table containing this address
	uint32_t table_idx = frame / PAGING_COUNTS;

	if (dir->tables[table_idx]) // page-table is already assigned
	{
		uint32_t page_idx = frame % PAGING_COUNTS;

		return &dir->tables[table_idx]->pages[page_idx];
	}
	else if (sMake)
	{
		return MakePageEntry(address, dir);
	}

	return 0;
}

Kernel::Memory::Paging::PageEntry *Kernel::Memory::Paging::MakePageEntry(uint32_t address, PageDirectory *dir)
{
	// address to an index
	uint32_t frame = address / PAGE_SIZE;

	// find the page table
	uint32_t table_idx = frame / PAGING_COUNTS;
	uint32_t page_idx = frame % PAGING_COUNTS;

	uint32_t physical_address;

	// retrieve a memory block which is page aligned, aswell as its physical address
	uint32_t virtual_address = kmalloc_(sizeof(Kernel::Memory::Paging::PageEntryTable), 1, &physical_address);
	memset(reinterpret_cast<unsigned char *>(virtual_address), 0, PAGE_SIZE); // fill with zeros

	// virtual address
	dir->tables[table_idx] = reinterpret_cast<Kernel::Memory::Paging::PageEntryTable *>(virtual_address);

	// physical address
	// set present, rw, and user-mode bits
	dir->tablePhysicals[table_idx] = physical_address | 0x7;

	// return what was made
	return &dir->tables[table_idx]->pages[page_idx];
}

static void SetFrame(uint32_t frame_address)
{
	uint32_t frame = frame_address / PAGE_SIZE;
	uint32_t index = (INDEX_FROM_BIT(frame));
	uint32_t offset = (OFFSET_FROM_BIT(frame));
	Kernel::Memory::Paging::frames.bitmap[index] |= 0x1 << offset;
}

static void ClearFrame(uint32_t frame_address)
{
	uint32_t frame = frame_address / PAGE_SIZE;
	uint32_t index = (INDEX_FROM_BIT(frame));
	uint32_t offset = (OFFSET_FROM_BIT(frame));
	Kernel::Memory::Paging::frames.bitmap[index] &= ~(0x1 << offset);
}

static bool FirstFrame(uint32_t &out)
{
	// loop through bitmap
	for (uint32_t i = 0; i < (INDEX_FROM_BIT(Kernel::Memory::Paging::frames.count)); i += 1)
	{
		// exit if nothing free
		if (Kernel::Memory::Paging::frames.bitmap[i] != 0xFFFFFFFF)
		{
			// loop through bits
			for (uint32_t j = 0; j < 32; j += 1)
			{
				uint32_t test = 0x1 << j;

				// if its free yoink it, huzzah!
				if (!(Kernel::Memory::Paging::frames.bitmap[i] & test))
				{
					out = i * 32 + j;
					return true;
				}
			}
		}
	}

	out = -1;
	return false;
}

void Kernel::Memory::Paging::AllocateFrame(Kernel::Memory::Paging::PageEntry *page, bool is_kernel, bool is_writeable)
{
	if (page->frame == 0)
	{
		uint32_t index = 0;

		if (!FirstFrame(index))
		{
			// no free pages
			for (;;)
			{
				asm volatile("hlt");
			}
		}

		// initialize a frame
		SetFrame(index * PAGE_SIZE);

		// add our flags
		page->present = 1;
		page->rw = (is_writeable) ? 1 : 0;
		page->user = (is_kernel) ? 0 : 1;
		page->frame = index;
	}
}

void Kernel::Memory::Paging::Init(uint32_t heap_start, uint32_t framebuffer_start, uint32_t framebuffer_size)
{
	// ngl idk how big this is.
	uint32_t physicalMemSize = 0x1000000;

	frames.count = physicalMemSize / PAGE_SIZE;

	// track allocated frames
	frames.bitmap = reinterpret_cast<uint32_t *>(kmalloc_(INDEX_FROM_BIT(frames.count), 0, 0));
	memset(reinterpret_cast<uint8_t *>(frames.bitmap), 0, INDEX_FROM_BIT(frames.count)); // fill with zeros

	// create a page directory and set it to zero
	uint32_t tmp = kmalloc_(sizeof(PageDirectory), 1, 0);
	memset(reinterpret_cast<uint8_t *>(tmp), 0, sizeof(PageDirectory)); // fill with zeros
	kernelDirectory = reinterpret_cast<PageDirectory *>(tmp);

	currentDirectory = kernelDirectory;
	
	// identity map our kernel.
	for (uint32_t i = 0; i < Kernel::Memory::KHeap::placementAddress + PAGE_SIZE; i += PAGE_SIZE)
	{
		AllocateFrame(GetPageEntry(i, kernelDirectory, 1), 0, 0);
	}

	for (uint32_t i = framebuffer_start; i < framebuffer_start + framebuffer_size; i += PAGE_SIZE)
	{
		AllocateFrame(GetPageEntry(i, kernelDirectory, 1), 0, 1);
	}

	// identity map our heap.
	for (uint32_t i = heap_start; i < heap_start + KHEAP_INITIAL_SIZE; i += PAGE_SIZE)
	{
		AllocateFrame(GetPageEntry(i, kernelDirectory, 1), 0, 0);
	}

	SwitchPageDirectory(kernelDirectory);
	Enable();

	uint32_t test = *((uint32_t*)framebuffer_start);
}

void Kernel::Memory::Paging::Enable()
{
	uint32_t cr0;

	// enable paging
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000; // set paging bit
	asm volatile("mov %0, %%cr0" : : "r"(cr0));
}

void Kernel::Memory::Paging::SwitchPageDirectory(Kernel::Memory::Paging::PageDirectory *dir)
{
	Kernel::Memory::Paging::currentDirectory = dir;

	// CR3 = address of tablePhysicals
	asm volatile("mov %0, %%cr3" : : "r"(&dir->tablePhysicals));
}

void Kernel::Memory::Paging::FreeFrame(Kernel::Memory::Paging::PageEntry *page)
{
	uint32_t frame = page->frame;

	if (!frame)
	{
		return; // frame was not previously allocated
	}
	else
	{
		ClearFrame(frame);
		page->frame = 0;
	}
}
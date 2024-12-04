#include "paging.hpp"

#include "../../../chelpers/memory.h"

#include "../kheap/kheap.hpp"

#define INDEX_FROM_BIT(a) (a / 32)
#define OFFSET_FROM_BIT(a) (a % 32)

namespace Kernel
{
	namespace Memory
	{
		namespace Paging
		{
			PageDirectory *currentDirectory;
			PageDirectory *kernelDirectory;
		}
	}
}

namespace Frame
{
	uint32_t count = 0;
	uint32_t bitmapSize = 0;
	uint32_t *bitmap = nullptr;
};

extern "C" void printf(const char *format, ...);

extern "C" void _enable_paging(void);
void _switch_page_directory(Kernel::Memory::Paging::PageDirectory *dir)
{
	Kernel::Memory::Paging::currentDirectory = dir;
	asm volatile("mov %0, %%cr3" : : "r"(&dir->physicals));
}

void SetupBitmap(const uint32_t memsize = 0x1000000){
	Frame::count = memsize / 0x1000;
	Frame::bitmapSize = INDEX_FROM_BIT(Frame::count);
	Frame::bitmap = reinterpret_cast<uint32_t *>(Kernel::Memory::KHeap::Early::pkmalloc_(Frame::bitmapSize));
	memset(reinterpret_cast<uint8_t *>(Frame::bitmap), 0, INDEX_FROM_BIT(Frame::count));

	printf("Paging Bitmap at 0x%X\n\tSize: 0x%X\n", Frame::bitmap, INDEX_FROM_BIT(Frame::count));
}

Kernel::Memory::Paging::Page *Kernel::Memory::Paging::GetPage(uint32_t address, Kernel::Memory::Paging::PageDirectory *dir, bool sMake, uint32_t *out_physical_address)
{
	// turn the address into an index
	uint32_t frame = address / 0x1000;

	// Find the page table containing this address
	uint32_t table_idx = frame / 1024;

	if (dir->tables[table_idx]) // page-table is already assigned
	{
		uint32_t page_idx = frame % 1024;

		return &dir->tables[table_idx]->pages[page_idx];
	}
	else if (sMake)
	{
		return MakePage(address, dir);
	}

	return 0;
}

Kernel::Memory::Paging::Page *Kernel::Memory::Paging::MakePage(uint32_t address, PageDirectory *dir)
{
	// address to an index
	uint32_t frame = address / 0x1000;

	// find the page table
	uint32_t table_idx = frame / 1024;
	uint32_t page_idx = frame % 1024;

	uint32_t physical_address;

	// retrieve a memory block which is page aligned, aswell as its physical address
	uint32_t virtual_address = kmalloc_(sizeof(Kernel::Memory::Paging::PageTable), 1, &physical_address);
	memset(reinterpret_cast<unsigned char *>(virtual_address), 0, 0x1000); // fill with zeros

	// virtual address
	dir->tables[table_idx] = reinterpret_cast<Kernel::Memory::Paging::PageTable *>(virtual_address);

	// physical address
	// set present, rw, and user-mode bits
	dir->physicals[table_idx] = physical_address | 0x7;

	// return what was made
	return &dir->tables[table_idx]->pages[page_idx];
}

static void SetFrame(uint32_t frame_address)
{
	uint32_t frame = frame_address / 0x1000;
	uint32_t index = (INDEX_FROM_BIT(frame));
	uint32_t offset = (OFFSET_FROM_BIT(frame));
	Frame::bitmap[index] |= 0x1 << offset;
}

static void ClearFrame(uint32_t frame_address)
{
	uint32_t frame = frame_address / 0x1000;
	uint32_t index = (INDEX_FROM_BIT(frame));
	uint32_t offset = (OFFSET_FROM_BIT(frame));
	Frame::bitmap[index] &= ~(0x1 << offset);
}

static bool FirstFrame(uint32_t &out)
{
	// loop through bitmap
	for (uint32_t i = 0; i < Frame::count; i += 1)
	{
		// exit if nothing free
		if (Frame::bitmap[i] != 0xFFFFFFFF)
		{
			// loop through bits
			for (uint32_t j = 0; j < 32; j += 1)
			{
				uint32_t test = 0x1 << j;

				// if its free yoink it, huzzah!
				if (!(Frame::bitmap[i] & test))
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

void Kernel::Memory::Paging::AllocateFrame(Kernel::Memory::Paging::Page *page, bool is_kernel, bool is_writeable)
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
		SetFrame(index * 0x1000);

		// add our flags
		page->present = 1;
		page->rw = (is_writeable) ? 1 : 0;
		page->user = (is_kernel) ? 0 : 1;
		page->frame = index;
	}
}

void Kernel::Memory::Paging::Init()
{
	uint32_t tmp = Kernel::Memory::KHeap::Early::pkmalloc_(sizeof(PageDirectory), true);
	memset(reinterpret_cast<uint8_t *>(tmp), 0, sizeof(PageDirectory)); // fill with zeros
	kernelDirectory = reinterpret_cast<PageDirectory *>(tmp);

	printf("Kernel Directory at 0x%X\n", kernelDirectory);

	SetupBitmap();
	currentDirectory = kernelDirectory;
}
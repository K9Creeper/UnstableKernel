#include "paging.hpp"

#include "../../../chelpers/memory.h"

#include "../../memory/memory.hpp"

#include "../pmm/pmm.hpp"
#include "../kheap/kheap.hpp"

#define IS_ALIGN(addr) ((((uint32_t)(addr)) | 0xFFFFF000) == 0)
#define PAGE_ALIGN(addr) ((((uint32_t)(addr)) & 0xFFFFF000) + 0x1000)

#define PAGEDIR_INDEX(vaddr) (((uint32_t)vaddr) >> 22)
#define PAGETBL_INDEX(vaddr) ((((uint32_t)vaddr) >>12) & 0x3ff)
#define PAGEFRAME_INDEX(vaddr) (((uint32_t)vaddr) & 0xfff)

namespace Kernel
{
	namespace MemoryManagement
	{
		namespace Paging
		{
			bool bIntialized = false;
            bool bEnabled = false;

			PageDirectory *currentDirectory;
			PageDirectory *kernelDirectory;
		}
	}
}

extern "C" void printf(const char *format, ...);

extern "C" void _enable_paging(void);
void _switch_page_directory(Kernel::MemoryManagement::Paging::PageDirectory *dir)
{
	Kernel::MemoryManagement::Paging::currentDirectory = dir;
	asm volatile("mov %0, %%cr3" : : "r"(&dir->physicals));
}

void SetupPMM(uint32_t mem_size){
	const uint32_t total_blocks = mem_size / 0x1000;
	Kernel::MemoryManagement::PMM::bitmap.SetSize(total_blocks);
	

	if(Kernel::MemoryManagement::PMM::bitmap.GetMemSize() * 8 < total_blocks)
	{
		Kernel::MemoryManagement::PMM::bitmap.SetMemSize(Kernel::MemoryManagement::PMM::bitmap.GetMemSize()+1);
	}

	Kernel::MemoryManagement::PMM::bitmap.Clear();

	 for(int i = 0; i < Kernel::MemoryManagement::PMM::bitmap.GetMemSize(); i++) {
        if(Kernel::MemoryManagement::PMM::bitmap.Get(i) != 0) {
			// This is not ideal...
        }
    }
}

void AllocatePage(Kernel::MemoryManagement::Paging::PageDirectory * dir, uint32_t virtual_addr, uint32_t frame, int is_kernel, int is_writable) {
	 // make pmm get a physical block, then assign the physical block to the virtual address

    Kernel::MemoryManagement::Paging::PageTable * table = nullptr;

    uint32_t page_dir_idx = PAGEDIR_INDEX(virtual_addr);
	uint32_t page_tbl_idx = PAGETBL_INDEX(virtual_addr);

    // If the coressponding page table does not exist, malloc!
    table = dir->ref_tables[page_dir_idx];
    if(!table) {
        if(!Kernel::MemoryManagement::KHeap::bInitialized)
            table = reinterpret_cast<Kernel::MemoryManagement::Paging::PageTable*>(Kernel::MemoryManagement::KHeap::Early::pkmalloc_(sizeof(Kernel::MemoryManagement::Paging::PageTable), true));
        else
            table = kmalloc_a(sizeof(Kernel::MemoryManagement::Paging::PageTable));

        memset(reinterpret_cast<unsigned char*>(table), 0, sizeof(Kernel::MemoryManagement::Paging::PageTable));
        // Remember, dumb_kmalloc returns a virtual address, but what we put into the paging structure, MUST BE, in terms of phsical address
        // Since we've mapped [0 to 4mb physical mem] to [0xc0000000 to 0xc0000000+4mb], we can get the physical addr by subtracting 0xc0000000

        uint32_t t = (uint32_t)virtual2phys(kpage_dir, table);
        dir->tables[page_dir_idx].frame = t >> 12;
        dir->tables[page_dir_idx].present = 1;
        dir->tables[page_dir_idx].rw = 1;
        dir->tables[page_dir_idx].user = 1;
        dir->tables[page_dir_idx].page_size = 0;
        // Leave a reference here so that later we can access this table
        dir->ref_tables[page_dir_idx] = table;
    }

    // If the coressponding page does not exist, allocate_block!
    if(!table->pages[page_tbl_idx].present) {
        uint32_t t;
        // Normally, we'll allocate frames from physical memory manager, but sometimes it's useful to be able to set any frame(for example, share memory between process)
        if(frame)
            t = frame;
        else
            t = allocate_block();
        table->pages[page_tbl_idx].frame = t;
        table->pages[page_tbl_idx].present = 1;
        table->pages[page_tbl_idx].rw = 1;
        table->pages[page_tbl_idx].user = 1;
    }
}

void Kernel::MemoryManagement::Paging::Init(uint32_t mem_size)
{
	SetupPMM(mem_size);

	uint32_t tmp = Kernel::MemoryManagement::KHeap::Early::pkmalloc_(sizeof(PageDirectory), true);
	memset(reinterpret_cast<uint8_t *>(tmp), 0, sizeof(PageDirectory)); // fill with zeros
	kernelDirectory = reinterpret_cast<PageDirectory *>(tmp);

	printf("Kernel Directory at 0x%X, size of 0x%X\n", kernelDirectory, sizeof(PageDirectory));

	currentDirectory = kernelDirectory;

	uint32_t i = Kernel::Memory::Info::kernel_start;

    while(i < (Kernel::Memory::Info::kernel_end > Kernel::Memory::Info::kernel_start + (4 * 1024 * 1024) ? Kernel::Memory::Info::kernel_end : Kernel::Memory::Info::kernel_start + (4 * 1024 * 1024))) {
        AllocatePage(kernelDirectory, i, 0, 1, 1);
        i = i + 0x1000;
    }

	bIntialized = true;
}
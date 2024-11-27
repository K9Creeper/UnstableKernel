#include "paging.hpp"

#include "../kheap/kheap.hpp"

#include "../../../c_helpers/memory.h"

extern "C" uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t * physAddress);

Kernel::Memory::Paging::PageEntry* Kernel::Memory::Paging::GetPageEntry ( uint32_t address, int make, Kernel::Memory::Paging::PageDirectory *dir )
{
	// Turn the address into an index
	uint32_t frame = address / PAGE_SIZE;

	// Find the page table containing this address
	uint32_t table_idx = frame / PAGING_COUNTS;
	uint32_t page_idx = frame % PAGING_COUNTS;

	if ( dir -> tables[ table_idx ] )  // page-table is already assigned
	{
		return &dir -> tables[ table_idx ] -> pages[ page_idx ];
	}
	else if ( make )  // create page-table
	{
		uint32_t physical_address;
		uint32_t virtual_address; 

		// Retrieve a memory block which is page aligned and its physical address
		virtual_address = kmalloc_( sizeof( Kernel::Memory::Paging::PageEntryTable ), 1, &physical_address );
		memset( reinterpret_cast<unsigned char*>(virtual_address), 0, PAGE_SIZE );  // fill with zeros
		
		// virtual address
		dir->tables[ table_idx ] = reinterpret_cast<Kernel::Memory::Paging::PageEntryTable*>(virtual_address);

		// physical address
		dir->tablePhysicals[ table_idx ] = physical_address | 0x7;  // set present, rw, and user-mode bits

		return &dir -> tables[ table_idx ] -> pages[ page_idx ];
	}
	else
	{
		return 0;
	}
}

static void SetFrame( uint32_t frame_address )
{
	uint32_t frame = frame_address / PAGE_SIZE;
	uint32_t index = (frame / 32);
	uint32_t offset = (frame % 32);
	Kernel::Memory::Paging::frames.bitmap[ index ] |= 0x1 << offset;
}

static void ClearFrame( uint32_t frame_address )
{
	uint32_t frame     = frame_address / PAGE_SIZE;  // ?
	uint32_t index = (frame / 32);
	uint32_t offset = (frame % 32);
	Kernel::Memory::Paging::frames.bitmap[ index ] &= ~ ( 0x1 << offset );
}

static bool FirstFrame(uint32_t* out)
{
	uint32_t i, j;

	for ( i = 0; i < Kernel::Memory::Paging::frames.count/32; i += 1 )
	{
		if ( Kernel::Memory::Paging::frames.bitmap[ i ] != 0xFFFFFFFF )  // exit early if nothing free
		{
			for ( j = 0; j < 32; j += 1 )
			{
				uint32_t test = 0x1 << j;

				if ( ! ( Kernel::Memory::Paging::frames.bitmap[ i ] & test ) )  // if free (zero)
				{
                    *out = i * 32 + j;
					return true;
				}
			}
		}
	}

	return false;
}

void Kernel::Memory::Paging::AllocateFrame(Kernel::Memory::Paging::PageEntry *page, bool is_kernel, bool is_writeable){
    if ( page->frame != 0 )
	{
        // already allocated
		return;  
	}
	else
	{
        // index of the first free frame
		uint32_t index = 0;  

		if (!FirstFrame(&index))
		{
            // no free pages. hang it.
			for(;;){ }
		}

        // initialize a frame
		SetFrame( index * PAGE_SIZE ); 

		page -> present = 1;
		page -> rw      = ( is_writeable ) ? 1 : 0;
		page -> user    = ( is_kernel ) ? 0 : 1;
		page -> frame   = index;
	}
}

void Kernel::Memory::Paging::Init(){
    uint32_t memSize = 0x1000000;
	frames.count = memSize / PAGE_SIZE;

    frames.bitmap = reinterpret_cast<uint32_t*>(kmalloc_( frames.count/32, 0, 0));
    memset( reinterpret_cast<unsigned char*>(frames.bitmap), 0, frames.count/32 );

    uint32_t tmp = kmalloc_(sizeof( Kernel::Memory::Paging::PageDirectory ), true, 0);
    memset( reinterpret_cast<unsigned char*>(tmp), 0, sizeof( Kernel::Memory::Paging::PageDirectory ) );
    Kernel::Memory::Paging::kernelDirectory = reinterpret_cast<Kernel::Memory::Paging::PageDirectory *>(tmp);

    Kernel::Memory::Paging::currentDirectory = Kernel::Memory::Paging::kernelDirectory;

    for (int i = 0xC0000000; i < 0xC0000000 + 0x100000; i += PAGE_SIZE )
	{
		GetPageEntry( i, 1, Kernel::Memory::Paging::kernelDirectory );
	}

    for(int i = 0; i < Kernel::Memory::KHeap::placementAddress + PAGE_SIZE; )
	{
		// Kernel code is readable but not writeable from userspace
		Kernel::Memory::Paging::AllocateFrame(Kernel::Memory::Paging::GetPageEntry( i, 1, Kernel::Memory::Paging::kernelDirectory ), 0, 0 );

		i += PAGE_SIZE;
	}

    for (int i = 0xC0000000; i < 0xC0000000 + 0x100000; i += PAGE_SIZE )
	{
		Kernel::Memory::Paging::AllocateFrame(Kernel::Memory::Paging::GetPageEntry( i, 1, Kernel::Memory::Paging::kernelDirectory ), 0, 0 );
	}


    SwitchPageDirectory(Kernel::Memory::Paging::kernelDirectory);
    Enable();
                           // KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE
    Kernel::Memory::KHeap::Init(0xC0000000, 0xC0000000 + 0x100000, 0xCFFFF000, 0, 0 );
}

void Kernel::Memory::Paging::Enable(){
    uint32_t cr0;
    // Enable paging
	asm volatile( "mov %%cr0, %0" : "=r" ( cr0 ) );
	cr0 |= 0x80000000;  // set paging bit
	asm volatile( "mov %0, %%cr0" : : "r" ( cr0 ) );
}

void Kernel::Memory::Paging::SwitchPageDirectory(Kernel::Memory::Paging::PageDirectory* dir){
	Kernel::Memory::Paging::currentDirectory = dir;

	// CR3 = address of tablePhysicals
	asm volatile( "mov %0, %%cr3" : : "r" ( &dir->tablePhysicals ) );
}

void Kernel::Memory::Paging::FreeFrame(Kernel::Memory::Paging::PageEntry *page){
	uint32_t frame = page->frame;

	if ( !frame)
	{
		return;  // frame was not previously allocated
	}
	else
	{
		ClearFrame(frame);
		page -> frame = 0;
	}
}
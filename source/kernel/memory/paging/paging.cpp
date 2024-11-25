#include "paging.hpp"

extern "C" void loadPageDirectory(unsigned int*);
extern "C" void enablePaging();

void Paging_Map_Virtual_To_Physical(uint32_t virt, uint32_t phys, bool readOnly = false)
{
    uint32_t flags = readOnly ? 1 : 3;  // Read only or read and write
	uint16_t id = virt >> 22;
	for(int i = 0; i < 1024; i++)
	{
		Kernel::Memory::Paging::lastPage[i] = phys | flags;
		phys += 4096;
	}

	Kernel::Memory::Paging::pageDirectory[id] = (reinterpret_cast<uint32_t>(Kernel::Memory::Paging::lastPage)) | 3;
	Kernel::Memory::Paging::lastPage = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(Kernel::Memory::Paging::lastPage) + 4096);
}

void Kernel::Memory::Paging::Init(){
    Kernel::Memory::Paging::pageDirectory = reinterpret_cast<uint32_t*>(0x400000);
    Kernel::Memory::Paging::lastPage = reinterpret_cast<uint32_t*>(0x404000);

    for(int i = 0; i < 1024; i++)
	{
		Kernel::Memory::Paging::pageDirectory[i] = 0 | 2;
	}

    Paging_Map_Virtual_To_Physical(0, 0);
	Paging_Map_Virtual_To_Physical(0x400000, 0x400000);

    loadPageDirectory(reinterpret_cast<unsigned int*>(Kernel::Memory::Paging::pageDirectory));
    enablePaging();
}
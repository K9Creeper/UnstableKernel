#include "paging.hpp"

extern "C" void loadPageDirectory(unsigned int*);
extern "C" void enablePaging();
extern "C" void disablePaging();

void Paging_Map_Virtual_To_Physical(uint32_t virt, uint32_t phys, bool readOnly = false)
{
    uint32_t flags = readOnly ? 1 : 3; 
    uint16_t id = virt >> 22;  
    
    uint32_t* pageTable = Kernel::Memory::Paging::lastPage;
    
    for (int i = 0; i < 1024; i++)
    {
        pageTable[i] = (phys + i * 4096) | flags;
    }

    Kernel::Memory::Paging::pageDirectory[id] = reinterpret_cast<uint32_t>(pageTable) | 3;  // Set the page directory entry with present and writable flags
    
    Kernel::Memory::Paging::lastPage += 1024;
}


void Kernel::Memory::Paging::Init()
{
	disablePaging();

    Kernel::Memory::Paging::pageDirectory = reinterpret_cast<uint32_t*>(0xC0001000);
    Kernel::Memory::Paging::lastPage = reinterpret_cast<uint32_t*>(0xC0010000);

    for (int i = 0; i < 1024; i++) {
        Kernel::Memory::Paging::pageDirectory[i] = 0 | 2;  // Unused entries marked as not present
    }

    Paging_Map_Virtual_To_Physical(0, 0);

    Paging_Map_Virtual_To_Physical(0xC0000000, 0xC0000000);

    loadPageDirectory(reinterpret_cast<unsigned int*>(Kernel::Memory::Paging::pageDirectory));
    enablePaging();
}

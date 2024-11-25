#include "kheap.hpp"

#include "../../../c_helpers/memory.h"

/// Take a look here: https://github.com/levex/osdev/blob/master/memory/malloc.c

namespace Kernel{
    namespace Memory{
        namespace KHeap{
            uint32_t lastAlloc = 0;
            uint32_t memoryUsed = 0;
        }
    }
}

struct Alloc_t{
	uint8_t status;
	uint32_t size;
};

void Kernel::Memory::KHeap::Init(uint32_t kernel_end){
    Kernel::Memory::KHeap::lastAlloc = kernel_end + 0x1000;
    Kernel::Memory::KHeap::heap::first = reinterpret_cast<uint32_t*>(Kernel::Memory::KHeap::lastAlloc);
    Kernel::Memory::KHeap::paHeap::last = reinterpret_cast<uint32_t*>(0x400000);
    Kernel::Memory::KHeap::paHeap::first = Kernel::Memory::KHeap::paHeap::last - (32 * 4096);
    Kernel::Memory::KHeap::heap::last = Kernel::Memory::KHeap::paHeap::first;
    memset(reinterpret_cast<unsigned char*>(Kernel::Memory::KHeap::heap::first), 0, Kernel::Memory::KHeap::heap::last - Kernel::Memory::KHeap::heap::first);
    Kernel::Memory::KHeap::paHeap::desc = reinterpret_cast<uint8_t*>(Kernel::Memory::KHeap::Malloc(32));

}

void Kernel::Memory::KHeap::Free(void* mem){
    Alloc_t* alloc = reinterpret_cast<Alloc_t*>(reinterpret_cast<unsigned int>(mem) - sizeof(Alloc_t));
	Kernel::Memory::KHeap::memoryUsed -= alloc->size + sizeof(Alloc_t);
	alloc->status = 0;
}

void* Kernel::Memory::KHeap::Malloc(unsigned int size){
    if(!size) return 0;

	uint8_t *mem = reinterpret_cast<uint8_t *>(Kernel::Memory::KHeap::heap::first);
	while(reinterpret_cast<uint32_t>(mem) < Kernel::Memory::KHeap::lastAlloc)
	{
		Alloc_t *a = reinterpret_cast<Alloc_t*>(mem);

		if(!a->size)
        {
            if(Kernel::Memory::KHeap::lastAlloc + size + sizeof(Alloc_t) >= reinterpret_cast<unsigned int>(Kernel::Memory::KHeap::heap::last))
	        {
		        for(;;){

				}
	        }
	        Alloc_t *alloc = reinterpret_cast<Alloc_t *>(Kernel::Memory::KHeap::lastAlloc);
	        alloc->status = 1;
	        alloc->size = size;

	        Kernel::Memory::KHeap::lastAlloc += size;
	        Kernel::Memory::KHeap::lastAlloc += sizeof(Alloc_t);
	        Kernel::Memory::KHeap::lastAlloc += 4;

	        Kernel::Memory::KHeap::memoryUsed += size + 4 + sizeof(Alloc_t);
	        memset(reinterpret_cast<unsigned char *>(reinterpret_cast<uint32_t>(alloc) + sizeof(Alloc_t)), 0, size);
	        return reinterpret_cast<void*>(reinterpret_cast<uint32_t>(alloc) + sizeof(Alloc_t));
        }
        
		if(a->status) {
			mem += a->size;
			mem += sizeof(Alloc_t);
			mem += 4;
			continue;
		}
        
		if(a->size >= size)
		{
			a->status = 1;

			memset(mem + sizeof(Alloc_t), 0, size);
			Kernel::Memory::KHeap::memoryUsed += size + sizeof(Alloc_t);
			return reinterpret_cast<void*>(mem + sizeof(Alloc_t));
		}
        
		mem += a->size;
		mem += sizeof(Alloc_t);
		mem += 4;
	}
}

void Kernel::Memory::KHeap::PAFree(void *mem){
    if(reinterpret_cast<uint32_t>(mem) < reinterpret_cast<unsigned int>(Kernel::Memory::KHeap::paHeap::first) || reinterpret_cast<uint32_t>(mem) > reinterpret_cast<unsigned int>(Kernel::Memory::KHeap::paHeap::last)) return;
	
    /* Determine which page is it */
	uint32_t ad = reinterpret_cast<uint32_t>(mem);
	ad -= reinterpret_cast<uint32_t>(Kernel::Memory::KHeap::paHeap::first);
	ad /= 4096;

	Kernel::Memory::KHeap::paHeap::desc[ad] = 0;
	return;
}

void* Kernel::Memory::KHeap::PAMalloc(unsigned int size){
    for(int i = 0; i < 32; i++)
	{
		if(Kernel::Memory::KHeap::paHeap::desc[i]) continue;
		Kernel::Memory::KHeap::paHeap::desc[i] = 1;

		return reinterpret_cast<void*>(reinterpret_cast<unsigned int>(Kernel::Memory::KHeap::paHeap::first) + i*4096);
	}
    // FATAL: failure
	return 0;
}
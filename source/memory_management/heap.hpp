/// --------
/// heap.hpp
/// @brief This file defines the structures related to heaps in the system.

#pragma once

#include "heap_ordered.hpp"

struct Header
{
    uint32_t magic;

    bool is_hole;
    uint32_t size;
};

struct Footer
{
    uint32_t magic;
    Header* header;
};

class Paging;

class Heap
{
protected:
    Paging* pManager;
    uint32_t nextPlacementAddress = 0;

private:
    bool bInitialized = false;

    bool bPreInitialized = false;

    HeapOrderedArray heapIndex;

    bool bSupervisor;
    bool bReadOnly;

    // helpers
    int FindSmallestHole(uint32_t size, bool page_align);
    void Expand(uint32_t new_size);
    uint32_t Contract(uint32_t new_size);
    void* Alloc(uint32_t size, bool page_align);
    void Free(void *p);

    uint32_t heap_start;
    uint32_t heap_end;
    uint32_t heap_max_address;

public:
    uint32_t GetHeapStart()const;
    uint32_t GetHeapEnd()const;
    uint32_t GetHeapMax()const;

    void PreInit(uint32_t end);

    void Init(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly, Paging* pMan = nullptr);

    uint32_t malloc_(uint32_t size, bool shouldAlign = false, uint32_t* physAddress = nullptr);
    void free(uint32_t address);
};
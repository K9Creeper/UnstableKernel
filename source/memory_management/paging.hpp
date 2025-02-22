/// ----------
/// paging.hpp
/// @brief This file declares and defines important structures relating to the core function of paging.
#pragma once

#include <stdint.h>

struct PageDirectoryEntry
{
    unsigned int present : 1;
    unsigned int rw : 1;
    unsigned int user : 1;
    unsigned int w_through : 1;
    unsigned int cache : 1;
    unsigned int access : 1;
    unsigned int reserved : 1;
    unsigned int page_size : 1;
    unsigned int global : 1;
    unsigned int available : 3;
    unsigned int frame : 20;
};

struct PageEntry
{
    unsigned int present : 1;
    unsigned int rw : 1;
    unsigned int user : 1;
    unsigned int reserved : 2;
    unsigned int accessed : 1;
    unsigned int dirty : 1;
    unsigned int reserved2 : 2;
    unsigned int available : 3;
    unsigned int frame : 20;
};

struct PageTable
{
    PageEntry pages[1024];
};

struct PageDirectory
{
    PageDirectoryEntry tables[1024];
    PageTable *ref_tables[1024];
};

class Heap;

class Paging
{
protected:
    Heap* heap;
    PageDirectory* dir;
private:
    
    bool bInitialzed;

    void DisablePSEReg();

    PageTable* CopyPageTable(PageDirectory *dst_page_dir, uint32_t page_dir_idx, PageTable *src);
public:
    void Init(uint32_t directoryLoc, Heap* heap, bool shouldClear = true);

    bool isEnabled()const;
    bool isInitialzed()const;

    void SwapHeap(Heap* heap);
    Heap* GetHeap()const;

    PageDirectory* GetDirectory()const;

    void EnablePaging();
    void SwitchToDirectory(bool isPhysical, uint32_t cr3 = 0);

    uint32_t Virtual2Phyiscal(uint32_t virtual_address, PageDirectory* diff = nullptr);

    void AllocateRegion(uint32_t start, uint32_t end, bool identity = false, bool isKernel = false, bool isWritable = false);

    void AllocatePage(uint32_t virtual_address, uint32_t frame, bool isKernel = false, int isWritable = false, PageDirectory* other = nullptr);
    void FreePage(uint32_t virtual_address, bool bFree);

    void CopyDirectory(PageDirectory *dst, PageDirectory * src = nullptr);
};
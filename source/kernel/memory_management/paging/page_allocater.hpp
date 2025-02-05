/// ------------------
/// page_allocater.hpp
/// This file declares functions and a class to manage and hold compile time allocated page directories.

#pragma once

#include "paging.hpp"

struct PageDirectoryAllocated{
    PageDirectory* directory;
    bool bUsed = false;
    bool PROTECT = false;
};

class PageAllocater {
    private:
        PageDirectoryAllocated raw_data[256];
        
    public:
        PageAllocater() : raw_data{nullptr} { }
        ~PageAllocater(){}

        void PreInit();
        void Init();
        
        PageDirectory* AllocatePageDirectory(bool PROTECTED = false);
        void FreePageDirectory(PageDirectory* loc);
};

extern PageAllocater PageDirectoryAllocater;
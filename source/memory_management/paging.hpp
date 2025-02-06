/// ----------
/// paging.hpp
/// @brief This file declares and defines important strucuters relating to the core function of paging.
#pragma once
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
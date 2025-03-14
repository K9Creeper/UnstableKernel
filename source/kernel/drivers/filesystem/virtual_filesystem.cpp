/// --------
/// virtual_filesystem.cpp
/// @brief This file will help control and use the virtual filesystem in the operating system.

#pragma once

#include "virtual_filesystem.hpp"

#include "../../../filesystem/virtual_filesystem.hpp"

#include "../../memory_management/kheap/kheap.hpp"

#include "../../../chelpers/memory.h"
#include "../../../chelpers/string.h"

extern "C" void printf(const char* f, ...);

static VirtualFileSystemNode* FindRecusion(const char* path, VirtualFileSystemNode* sub){
    printf("%s\n", path);

    if(strlen(path) <= 1 && (path[0] == '/' || path[0] == '\0')){
        return sub;
    }

    static char out[256];
    static char newPath[256];
    strtok(path, "/", out);

    int j = 0;
    for(int i = find(path, out) + strlen(out) + 1; i < strlen(path);i++)
    {
        newPath[j++] = path[i];
    }
    
    if(j >= strlen(path))
        j = strlen(path)-1;
    
    newPath[j] = '\0';
    
    strtok(newPath, "/", out);

    for(int i = 0; i < sub->children.GetMaxSize(); i++)
    {
        VirtualFileSystemNode* c = sub->children.Get(i);

        if(!c)
            continue;
        printf("%d %s %s\n", find(c->name, out), c->name, out);
        if(strlen(c->name) == strlen(out) && find(c->name, out) == 0)
            return FindRecusion(newPath, sub);
    }

    return nullptr;
}

VirtualFileSystemNode* VirtualFileSystemTree::Find(const char *path)
{
    
}

void VirtualFileSystemTree::Insert(VirtualFileSystemNode *sub, VirtualFileSystemNode *node)
{
    if (!sub)
    {
        this->root = node;
        return;
    }

    reinterpret_cast<VirtualFileSystemNode *>(node)->parent = sub;
}

void VirtualFileSystemTree::Remove(VirtualFileSystemNode *sub, VirtualFileSystemNode *node)
{

}

namespace Kernel
{
    namespace Drivers
    {
        namespace Filesystem
        {
            VirtualFileSystemTree* tree = nullptr;
        }
    }
}


void Kernel::Drivers::Filesystem::Init()
{
    Kernel::Drivers::Filesystem::tree = Kernel::MemoryManagement::kheap.malloc_(sizeof(VirtualFileSystemTree));

    Kernel::Drivers::Filesystem::tree->root = Kernel::MemoryManagement::kheap.malloc_(sizeof(VirtualFileSystemNode));
    memcpy(Kernel::Drivers::Filesystem::tree->root->name, "/root", strlen("/root"));

    VirtualFileSystemNode* lv1 = Kernel::MemoryManagement::kheap.malloc_(sizeof(VirtualFileSystemNode));
    memcpy(lv1->name, "/lv1", strlen("/lv1"));

    Kernel::Drivers::Filesystem::tree->root->children.Add(lv1);

    VirtualFileSystemNode* fin = FindRecusion("/root/lv1", Kernel::Drivers::Filesystem::tree->root);
    printf("Found %x\n", fin);

}


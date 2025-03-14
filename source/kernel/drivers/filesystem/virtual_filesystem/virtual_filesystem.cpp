/// ----------------------
/// virtual_filesystem.cpp
/// @brief This file will help control and use the virtual filesystem in the operating system.

#pragma once

#include "virtual_filesystem.hpp"

#include "../../../memory_management/kheap/kheap.hpp"

#include "../../../../chelpers/memory.h"
#include "../../../../chelpers/string.h"

extern "C" void printf(const char *f, ...);

static VirtualFileSystemNode *FindRecursion(const char *path, VirtualFileSystemNode *sub)
{
    int pathLen = strlen(path);

    if ((pathLen == 0 || (pathLen == 1 && (path[0] == '/' || path[0] == '\0'))) || (pathLen > 1 && find(&(path[1]), "/") == -1))
    {
        return sub;
    }

    static char out[256];
    static char newPath[256];
    strtok(path, "/", out);

    int j = 0;

    for (int i = find(path, out) + strlen(out); i < pathLen; i++)
    {
        newPath[j++] = path[i];
    }

    int nPathLen = strlen(newPath);
    if (nPathLen == 0 || (nPathLen == 1 && (newPath[0] == '/' || newPath[0] == '\0')))
        return sub;

    if (j >= pathLen)
        j = pathLen - 1;

    newPath[j] = '\0';

    strtok(newPath, "/", out);

    for (int i = 0; i < sub->children.GetMaxSize(); i++)
    {
        VirtualFileSystemNode *c = sub->children.Get(i);

        if (!c)
            continue;

        if (equal(c->name, out))
            return FindRecursion(newPath, c);
    }

    return nullptr;
}

VirtualFileSystemNode *VirtualFileSystemTree::Find(const char *path)
{
    return FindRecursion(path, root);
}

namespace Kernel
{
    namespace Drivers
    {
        namespace Filesystem
        {
            VirtualFileSystemTree *tree = nullptr;
        }
    }
}

static VirtualFileSystemNode *CreateNodeTo(VirtualFileSystemNode *parent, const char *filename)
{
    VirtualFileSystemNode *n = Kernel::MemoryManagement::kheap.malloc_(sizeof(VirtualFileSystemNode));
    memcpy(n->name, filename, strlen(filename));

    n->parent = parent;

    parent->children.Add(n);

    return n;
}

static VirtualFileSystemNode *CreateNodeTo(const char *parentPath, const char *filename)
{
    return CreateNodeTo(Kernel::Drivers::Filesystem::tree->Find(parentPath), filename);
}

static VirtualFileSystemNode *FileOpen(const char *filename, uint32_t flags)
{
    VirtualFileSystemNode *node = Kernel::Drivers::Filesystem::tree->Find(filename);

    if (!node)
        return nullptr;

    
        
    return node;
}

void Kernel::Drivers::Filesystem::Init()
{
    Kernel::Drivers::Filesystem::tree = Kernel::MemoryManagement::kheap.malloc_(sizeof(VirtualFileSystemTree));

    Kernel::Drivers::Filesystem::tree->root = Kernel::MemoryManagement::kheap.malloc_(sizeof(VirtualFileSystemNode));
    memcpy(Kernel::Drivers::Filesystem::tree->root->name, "/root/", strlen("/root/"));

    CreateNodeTo("/root", "/directory");

    FileOpen("/root/directory", 0);
}
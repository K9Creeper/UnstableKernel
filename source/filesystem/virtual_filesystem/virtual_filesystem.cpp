/// ----------------------
/// virtual_filesystem.cpp
/// @brief This file defines the functions of the virtual filesystem.

#include "virtual_filesystem.hpp"

bool VirtualFileSystemNodeList::Add(VirtualFileSystemNode *item)
{
    int i = 0;

    while (i < VFS_MAX_CHILDREN && this->Get(i))
    {
        i++;
    }

    if (i >= VFS_MAX_CHILDREN)
        return false;

    this->array[i] = item;

    return true;
}

VirtualFileSystemNode *VirtualFileSystemNodeList::Get(uint32_t i)
{
    return this->array[i];
}

void VirtualFileSystemNodeList::Remove(uint32_t i)
{
    this->array[i] = nullptr;
}

uint32_t VirtualFileSystemNodeList::GetMaxSize() const
{
    return VFS_MAX_CHILDREN;
}

void VirtualFileSystemTree::Insert(const char *parentPath, VirtualFileSystemNode *node)
{
    Insert(Find(parentPath), node);
}

void VirtualFileSystemTree::Insert(VirtualFileSystemNode *sub, VirtualFileSystemNode *node)
{
    if (!sub)
        return;

    for (int i = 0; i < sub->children.GetMaxSize(); i++)
    {
        // no dups
        if (sub->children.Get(i) == node)
            return;
    }

    sub->children.Add(node);
}

void VirtualFileSystemTree::Remove(VirtualFileSystemNode *sub, VirtualFileSystemNode *node)
{
    for (int i = 0; i < sub->children.GetMaxSize(); i++)
    {
        // find and goo
        if (sub->children.Get(i) == node)
        {
            sub->children.Remove(i);
            return;
        }
    }
}
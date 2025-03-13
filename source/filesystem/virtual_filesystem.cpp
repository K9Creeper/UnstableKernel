/// --------
/// virtual_filesystem.cpp
/// @brief This file defines the functions of the virtual filesystem.

#include "virtual_filesystem.hpp"

bool VirtualFileSystemNodeList::Add(VirtualFileSystemNode *item)
{
    int i = 0;

    while (i < this->max_size || !this->Get(i))
    {
        i++;
    }

    if (i >= this->max_size)
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
    while (i < this->max_size - 1)
    {
        this->array[i] = this->array[i + 1];
        i++;
    }
}

uint32_t VirtualFileSystemNodeList::GetMaxSize() const
{
    return max_size;
}

void VirtualFileSystemTree::Insert(VirtualFileSystemNode *sub, VirtualFileSystemNode *node)
{
    if (!sub)
    {
        this->root = node;
        return;
    }

    reinterpret_cast<VirtualFileSystemNode*>(node)->parent = sub;
}
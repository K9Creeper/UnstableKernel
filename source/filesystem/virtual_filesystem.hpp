/// --------
/// virtual_filesystem.hpp
/// @brief This file defines the structures of the virtual filesystem.

#pragma once

#include <stdint.h>

#include "ext2.hpp"

enum VirtualFileSystemNodeTypes : uint16_t{
    FileSystemNodeType_FIFO = 0x1000,
    FileSystemNodeType_CharacterDevice = 0x2000,
    FileSystemNodeType_Directory = 0x4000,
    FileSystemNodeType_BlockDevice = 0x6000,
    FileSystemNodeType_RegularFile = 0x8000,
    FileSystemNodeType_SymbolicLink = 0xA000,
};

struct VirtualFileSystemNode;

class VirtualFileSystemNodeList{
	protected:
        VirtualFileSystemNode* array[512];

		uint32_t max_size;
	public:
        VirtualFileSystemNodeList() : max_size{512}{}
		~VirtualFileSystemNodeList(){}

		bool Add(VirtualFileSystemNode* item);

		VirtualFileSystemNode* Get(uint32_t i);
		void Remove(uint32_t i);

		uint32_t GetMaxSize()const;
};

struct VirtualFileSystemNode{
    VirtualFileSystemNode* parent;
    VirtualFileSystemNodeList children;

    char name[256];
    void* device;
    uint32_t mask;
    uint32_t uid;
    uint32_t gid;
    uint32_t flags;
    uint32_t inode_num;
    uint32_t size;
    uint32_t fs_type;
    uint32_t open_flags;

    uint32_t create_time;
    uint32_t access_time;
    uint32_t modified_time;

    uint32_t offset;
    unsigned nlink;
    int refcount;
};

struct VirtualFileSystemTree{
    VirtualFileSystemNode* root;

    void Insert(VirtualFileSystemNode* sub, VirtualFileSystemNode* node);
};
/// --------
/// ext2.hpp
/// @brief This file defines the structures of the ext2 filesystem.

#pragma once

#include <stdint.h>

enum FileSystemState : uint16_t{
    FileSystemState_Clean = 1,
    FileSystemState_Error
}

enum FileSystemOSId : uin32_t{
    FileSystemOSId_Linux = 0,
    FileSystemOSId_GNU_HURD,
    FileSystemOSId_MASIX,
    FileSystemOSId_FreeBSD,
    FileSystemOSId_Other
}

struct Superblock{
    uint32_t inodes_count;
    uint32_t block_count;
    uint32_t su_blocks;
    uint32_t free_blocks;
    uint32_t free_inodes;
    uint32_t superblock_idx;
    uint32_t log2block_size;
    uint32_t log2frag_size;
    uint32_t blocks_per_group;
    uint32_t frags_per_group;
    uint32_t inodes_per_group;

    uint32_t mtime;
    uint32_t wtime;

    uint16_t mount_count;
    uint16_t mount_allowed_count;
    uint16_t ext2_magic;
    uint16_t fs_state;
    uint16_t err;
    uint16_t minor;

    uint32_t last_check;
    uint32_t interval;
    uint32_t os_id;
    uint32_t major;

    uint16_t r_userid;
    uint16_t r_groupid;

    char unused0[1024-84];
}__attribute__ ((packed));

struct BlockGroupDescriptor{
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t inode_table;
    uint32_t free_blocks;
    uint32_t free_inodes;
    uint32_t num_dirs;
    char unused0[8];
}__attribute__ ((packed));

enum FileSystemINodeTypes : uint16_t{
    FileSystemINodeType_FIFO = 0x1000,
    FileSystemINodeType_CharacterDevice = 0x2000,
    FileSystemINodeType_Directory = 0x4000,
    FileSystemINodeType_BlockDevice = 0x6000,
    FileSystemINodeType_RegularFile = 0x8000,
    FileSystemINodeType_SymbolicLink = 0xA000,
    FileSystemINodeType_UnixSockey = 0xC000
}

enum FileSystemINodePermisions : uint16_t{
    FileSystemINodePermisions_OExecute = 0x001,
    FileSystemINodePermisions_OWrite = 0x002,
    FileSystemINodePermisions_ORead = 0x004,
    FileSystemINodePermisions_GExecute = 0x008,
    FileSystemINodePermisions_GWrite = 0x010,
    FileSystemINodePermisions_GRead = 0x020,
    FileSystemINodePermisions_UExecute = 0x040,
    FileSystemINodePermisions_UWrite = 0x080,
    FileSystemINodePermisions_URead = 0x100,
    FileSystemINodePermisions_StickyBit = 0x200,
    FileSystemINodePermisions_SetGroupId = 0x400,
    FileSystemINodePermisions_SetUserId = 0x800
}

enum FileSystemINodeFlags : uint32_t{
    FileSystemINodeFlags_SynchronousUpdates = 0x00000008,
    FileSystemINodeFlags_Immutable = 0x00000010,
    FileSystemINodeFlags_Append = 0x00000020,
    FileSystemINodeFlags_NotDumpCmd = 0x00000040,
    FileSystemINodeFlags_AccessTimeNoUpdate = 0x00000080,
    FileSystemINodeFlags_HashIndexedDirectory = 0x00010000,
    FileSystemINodeFlags_AFSDirectory = 0x00020000,
    FileSystemINodeFlags_Journal = 0x00040000
}

struct INode {
    uint16_t types_permission;
    uint16_t userid;
    uint32_t size;
    uint32_t atime;
    uint32_t ctime;
    uint32_t mtime;
    uint32_t dtime;
    uint16_t gid;
    uint16_t hard_links;
    uint32_t num_sectors;
    uint32_t flags;
    uint32_t os_specific1;
    uint32_t blocks[EXT2_DIRECT_BLOCKS + 3];
    uint32_t generation;
    uint32_t file_acl;
    union {
        uint32_t dir_acl;
        uint32_t size_high;
    };
    uint32_t f_block_addr;
    char os_specific2[12];
}__attribute__ ((packed));

enum FileSystemDirectoryEntryType : uint8_t{
    FileSystemDirectoryEntryType_Unkown = 0,
    FileSystemDirectoryEntryType_Regular,
    FileSystemDirectoryEntryType_Directory,
    FileSystemDirectoryEntryType_CharacterDevice,
    FileSystemDirectoryEntryType_BlockDevice,
    FileSystemDirectoryEntryType_FIFO,
    FileSystemDirectoryEntryType_Socket,
    FileSystemDirectoryEntryType_SymbolicLink
}

struct DirectoryEntry{
    uint32_t inode,
    uint16_t size;
    uint8_t  name_len;
    uint8_t  type;
    char name[];
}
/// --------
/// ext2.hpp
/// @brief This file defines the structures of the ext2 filesystem.

#pragma once

#include <stdint.h>

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

struct EXT2FileSystem {
    void* disk_device;

    Superblock* sb;
    BlockGroupDescriptor* bgds;
    uint32_t block_size;
    uint32_t blocks_per_group;
    uint32_t inodes_per_group;
    uint32_t total_groups;

    uint32_t bgd_blocks;
};
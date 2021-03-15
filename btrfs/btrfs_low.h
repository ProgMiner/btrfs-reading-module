#pragma once

#include <sys/stat.h>

#include "struct/btrfs_super_block.h"
#include "btrfs_chunk_list.h"
#include "types.h"


/* unique identifier for any file in btrfs */
struct btrfs_low_file_id {

    /* bytenr of fs_tree root */
    u64 fs_tree;

    /* objectid of inode_item */
    u64 dir_item;
};

struct btrfs_super_block * btrfs_low_find_superblock(void * data);

struct btrfs_chunk_list * btrfs_low_read_sys_array(struct btrfs_super_block * sb);

struct btrfs_chunk_list * btrfs_low_read_chunk_tree(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 chunk_root
);

u64 btrfs_low_find_root_fs_tree_root(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 root
);

int btrfs_low_locate_file(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 root_tree,
        u64 fs_tree,
        const char * path,
        struct btrfs_low_file_id * result
);

int btrfs_low_stat(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        struct btrfs_low_file_id file_id,
        struct stat * stat
);

size_t btrfs_low_list_files(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        struct btrfs_low_file_id dir_id,
        const char *** files
);

size_t btrfs_low_read(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        struct btrfs_low_file_id file_id,
        char ** content
);

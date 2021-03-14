#pragma once

#include "struct/btrfs_super_block.h"
#include "btrfs_chunk_list.h"
#include "types.h"


/* unique identifier for any file in btrfs */
struct btrfs_low_dir_item_id {

    /* bytenr of fs_tree root */
    u64 fs_tree_root;

    /* objectid of dir_item */
    u64 dir_item_objectid;
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

struct btrfs_dir_item btrfs_low_find_dir_item(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        const char * path
);

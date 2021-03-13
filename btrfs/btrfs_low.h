#pragma once

#include "struct/btrfs_super_block.h"
#include "btrfs_traverse_btree.h"
#include "btrfs_chunk_list.h"
#include "types.h"


struct btrfs_super_block * btrfs_low_find_superblock(void * data);

struct btrfs_chunk_list * btrfs_read_sys_array(struct btrfs_super_block * sb);

struct btrfs_chunk_list * btrfs_read_chunk_tree(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 chunk_root
);

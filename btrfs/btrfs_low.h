#pragma once

#include "struct/btrfs_super_block.h"
#include "btrfs_chunk_list.h"


#define btrfs_traverse_btree(__chunk_list, __data, __btree_root, __acc, __handler) \
btrfs_traverse_btree_do(__chunk_list, __data, __btree_root, \
        __acc, (btrfs_traverse_btree_handler) __handler)
#define btrfs_traverse_btree_continue return BTRFS_TRAVERSE_BTREE_CONTINUE
#define btrfs_traverse_btree_break    return BTRFS_TRAVERSE_BTREE_BREAK


enum btrfs_traverse_btree_handler_result {
    BTRFS_TRAVERSE_BTREE_CONTINUE,
    BTRFS_TRAVERSE_BTREE_BREAK
};

typedef enum btrfs_traverse_btree_handler_result (* btrfs_traverse_btree_handler)(
        void * acc,
        struct btrfs_key item_key,
        void * item_data
);

struct btrfs_super_block * btrfs_low_find_superblock(void * data);

struct btrfs_chunk_list * btrfs_read_sys_array(struct btrfs_super_block * sb);

void btrfs_traverse_btree_do(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 btree_root,
        void * acc,
        btrfs_traverse_btree_handler handler
);

struct btrfs_chunk_list * btrfs_read_chunk_tree(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 chunk_root
);

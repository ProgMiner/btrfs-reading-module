#pragma once

#include "btrfs_chunk_list.h"
#include "struct/btrfs_key.h"
#include "types.h"


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
        const void * item_data
);

void btrfs_traverse_btree_do(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 btree_root,
        void * acc,
        btrfs_traverse_btree_handler handler
);

#ifdef BTRFS_DEBUG
void btrfs_traverse_btree_print(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 btree_root
);
#else
#define btrfs_traverse_btree_print(__chunk_list, __data, __btree_root)
#endif

#pragma once

#include "types.h"
#include "btrfs_chunk_list.h"
#include "struct/btrfs_key.h"


void * btrfs_find_in_btree(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 btree_root,
        struct btrfs_key key,
        struct btrfs_key * result
);

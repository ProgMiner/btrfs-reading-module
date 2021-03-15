#pragma once

#include "include/btrfs.h"
#include "btrfs_chunk_list.h"


struct btrfs {
    void * data;
    struct btrfs_chunk_list * chunk_list;
    u64 root_tree;
    u64 root_fs_tree;
};

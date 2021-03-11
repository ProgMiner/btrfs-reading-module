#pragma once

#include "include/btrfs.h"
#include "btrfs_chunk_list.h"


struct btrfs {
    void * data;
    struct btrfs_super_block * sb;
    struct btrfs_chunk_list * chunk_list;
};

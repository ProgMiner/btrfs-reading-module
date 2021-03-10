#pragma once

#include "include/btrfs.h"


struct btrfs {
    void * data;
    struct btrfs_super_block * sb;
};

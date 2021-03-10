#pragma once

#include "struct/btrfs_super_block.h"


struct btrfs_super_block * btrfs_low_find_superblock(void * data);

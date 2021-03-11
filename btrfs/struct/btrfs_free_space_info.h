#pragma once

#include "../types.h"


#define BTRFS_FREE_SPACE_USING_BITMAPS (1ULL << 0)


struct btrfs_free_space_info {

    /* number of extents that are tracking the free space for this block group */
    __le32 extent_count;

    /* flags associated with this free_space_info
     * (current it can be 0 or BTRFS_FREE_SPACE_USING_BITMAPS)
     */
    __le32 flags;
} __attribute__ ((__packed__));

#pragma once

#include "../types.h"


#define BTRFS_EXTENT_FLAG_DATA       0x1
#define BTRFS_EXTENT_FLAG_TREE_BLOCK 0x2

#define BTRFS_BLOCK_FLAG_FULL_BACKREF 0x80


struct btrfs_extent_item {

    /* the number of explicit references to this extent */
    __le64 refs;

    /* transid of transaction that allocated this extent */
    __le64 generation;

    /* flags
     *
     * Flags
     *   - BTRFS_EXTENT_FLAG_DATA [0x1]
     *     Flag to indicate that the following record refers to a data extent
     *   - BTRFS_EXTENT_FLAG_TREE_BLOCK [0x2]
     *     Flag to indicate that the following record refers to a metadata tree block
     *     - BTRFS_BLOCK_FLAG_FULL_BACKREF [0x80]
     *       Tree block back reference contains a full back reference.
     */
    __le64 flags;
} __attribute__ ((__packed__));

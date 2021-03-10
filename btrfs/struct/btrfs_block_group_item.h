#pragma once

#include "../types.h"


#define BTRFS_BLOCK_GROUP_DATA      0x1
#define BTRFS_BLOCK_GROUP_SYSTEM    0x2
#define BTRFS_BLOCK_GROUP_METADATA  0x4

#define BTRFS_BLOCK_GROUP_RAID0     0x008
#define BTRFS_BLOCK_GROUP_RAID1     0x010
#define BTRFS_BLOCK_GROUP_DUP       0x020
#define BTRFS_BLOCK_GROUP_RAID10    0x040
#define BTRFS_BLOCK_GROUP_RAID5     0x080
#define BTRFS_BLOCK_GROUP_RAID6     0x100


struct btrfs_block_group_item {

    /* the space used in this block group */
    __le64 used;

    /* the objectid of the chunk backing this block group */
    __le64 chunk_objectid;

    /* flags
     *
     * Allocation Type:
     * 
     * The type of storage this block group offers.
     * SYSTEM chunks cannot be mixed, but DATA and METADATA chunks can be mixed.
     *   - BTRFS_BLOCK_GROUP_DATA [0x1]
     *   - BTRFS_BLOCK_GROUP_SYSTEM [0x2]
     *   - BTRFS_BLOCK_GROUP_METADATA [0x4]
     *
     * Replication Policy:
     * 
     * The allocation policy this block group implements.
     * Only one of the following flags may be set in any single block group.
     * It is not possible to combine policies to create nested RAID levels beyond
     * the RAID-10 support offered below. If no flags are specified,
     * the block group is not replicated beyond a single, unstriped copy.
     *
     *   - BTRFS_BLOCK_GROUP_RAID0 [0x8]
     *     Striping (RAID-0)
     *
     *   - BTRFS_BLOCK_GROUP_RAID1 [0x10]
     *     Mirror on a separate device (RAID-1)
     *
     *   - BTRFS_BLOCK_GROUP_DUP [0x20]
     *     Mirror on a single device
     *
     *   - BTRFS_BLOCK_GROUP_RAID10 [0x40]
     *     Striping and mirroring (RAID-10)
     *
     *   - BTRFS_BLOCK_GROUP_RAID5 [0x80]
     *     Parity striping with single-disk fault tolerance (RAID-5)
     *
     *   - BTRFS_BLOCK_GROUP_RAID6 [0x100]
     *     Parity striping with double-disk fault tolerance (RAID-6)
     */
    __le64 flags;
} __attribute__ ((__packed__));

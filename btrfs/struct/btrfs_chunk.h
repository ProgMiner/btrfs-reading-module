#pragma once

#include "../types.h"
#include "btrfs_stripe.h"


struct btrfs_chunk {

    /* size of chunk, in bytes */
    __le64 length;

    /* objectid of the root referencing this chunk, always EXTENT_ROOT */
    __le64 owner;

    /* replication stripe length */
    __le64 stripe_len;

    /* uses the same flags as btrfs_block_group_item */
    __le64 type;

    /* optimal I/O alignment for this chunk */
    __le32 io_align;

    /* optimal I/O width for this chunk */
    __le32 io_width;

    /* minimal I/O size for this chunk */
    __le32 sector_size;

    /* number of replication stripes */
    __le16 num_stripes;

    /* number of replication sub-stripes (used only for RAID-10) */
    __le16 sub_stripes;

    /* the first of one or more stripes that map to device extents */
    struct btrfs_stripe stripe;
} __attribute__ ((__packed__));

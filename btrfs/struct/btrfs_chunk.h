#pragma once

#include <stddef.h>

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

static inline u64 btrfs_chunk_length(const struct btrfs_chunk * chunk) {
    return le64_to_cpu(chunk->length);
}

static inline u64 btrfs_chunk_stripe_len(const struct btrfs_chunk * chunk) {
    return le64_to_cpu(chunk->stripe_len);
}

static inline u16 btrfs_chunk_num_stripes(const struct btrfs_chunk * chunk) {
    return le16_to_cpu(chunk->num_stripes);
}

static inline size_t btrfs_chunk_size(const struct btrfs_chunk * chunk) {
    return sizeof(struct btrfs_chunk) + sizeof(struct btrfs_stripe)
            * (btrfs_chunk_num_stripes(chunk) - 1);
}

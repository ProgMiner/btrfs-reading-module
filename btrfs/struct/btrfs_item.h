#pragma once

#include "../types.h"
#include "btrfs_disk_key.h"


struct btrfs_item {
    struct btrfs_disk_key key;
    __le32 offset;
    __le32 size;
} __attribute__ ((__packed__));

static inline u32 btrfs_item_offset(struct btrfs_item * item) {
    return le32_to_cpu(item->offset);
}

static inline u32 btrfs_item_size(struct btrfs_item * item) {
    return le32_to_cpu(item->size);
}

#ifdef BTRFS_DEBUG
void btrfs_item_print(struct btrfs_item * item);
#else
#define btrfs_item_print(__item)
#endif

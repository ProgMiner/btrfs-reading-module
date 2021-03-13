#pragma once

#include "../types.h"
#include "../btrfs_debug.h"
#include "btrfs_disk_key.h"


struct btrfs_key_pointer {
    struct btrfs_disk_key key;
    __le64 blocknr;
    __le64 generation;
};

static inline u64 btrfs_key_pointer_blocknr(struct btrfs_key_pointer * key_pointer) {
    return le64_to_cpu(key_pointer->blocknr);
}

static inline u64 btrfs_key_pointer_generation(struct btrfs_key_pointer * key_pointer) {
    return le64_to_cpu(key_pointer->generation);
}

#ifdef BTRFS_DEBUG
void btrfs_key_pointer_print(struct btrfs_key_pointer * key_pointer);
#else
#define btrfs_key_pointer_print(__key_pointer)
#endif

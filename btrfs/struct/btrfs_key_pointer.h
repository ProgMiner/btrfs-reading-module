#pragma once

#include "../types.h"
#include "btrfs_disk_key.h"


struct btrfs_key_pointer {
    struct btrfs_disk_key key;
    __le64 blocknr;
    __le64 generation;
};

static inline u64 btrfs_key_pointer_blocknr(struct btrfs_key_pointer * key_pointer) {
    return le64_to_cpu(key_pointer->blocknr);
}

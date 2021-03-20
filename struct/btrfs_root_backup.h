#pragma once

#include "../types.h"


struct btrfs_root_backup {

    __le64 tree_root;

    __le64 tree_root_gen;

    __le64 chunk_root;

    __le64 chunk_root_gen;

    __le64 extent_root;

    __le64 extent_root_gen;

    __le64 fs_root;

    __le64 fs_root_gen;

    __le64 dev_root;

    __le64 dev_root_gen;

    __le64 csum_root;

    __le64 csum_root_gen;

    __le64 total_bytes;

    __le64 bytes_used;

    __le64 num_devices;

    /* future */
    __le64 unused_64[4];

    u8 tree_root_level;

    u8 chunk_root_level;

    u8 extent_root_level;

    u8 fs_root_level;

    u8 dev_root_level;

    u8 csum_root_level;

    /* future and to align */
    u8 unused_8[10];
} __attribute__ ((__packed__));

#pragma once

#include "../types.h"


struct btrfs_free_space_header {

    struct btrfs_disk_key location;

    __le64 generation;

    __le64 num_entries;

    __le64 num_bitmaps;
} __attribute__ ((__packed__));

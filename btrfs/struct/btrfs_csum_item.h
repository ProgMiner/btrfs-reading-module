#pragma once

#include "../types.h"


struct btrfs_csum_item {
    u8 csum;
} __attribute__ ((__packed__));

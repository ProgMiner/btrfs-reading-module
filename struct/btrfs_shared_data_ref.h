#pragma once

#include "../types.h"


struct btrfs_shared_data_ref {

    /* number of references held */
    __le32 count;
} __attribute__ ((__packed__));

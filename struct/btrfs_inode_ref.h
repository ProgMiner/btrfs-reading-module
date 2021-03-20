#pragma once

#include "../types.h"


struct btrfs_inode_ref {

    /* index of the inode this item's referencing in the directory */
    __le64 index;

    /* length of the name, following this item */
    __le16 name_len;

    /* name goes here */
} __attribute__ ((__packed__));

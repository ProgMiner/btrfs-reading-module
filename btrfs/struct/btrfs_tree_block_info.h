#pragma once

#include "../types.h"


struct btrfs_tree_block_info {

    /* although this value may be accurate, it's unused */
	struct btrfs_disk_key key;

    /* level of the tree that contains this node */
	u8 level;
} __attribute__ ((__packed__));

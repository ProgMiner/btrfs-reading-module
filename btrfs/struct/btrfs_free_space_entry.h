#pragma once

#include "../types.h"


#define BTRFS_FREE_SPACE_EXTENT	1
#define BTRFS_FREE_SPACE_BITMAP	2


struct btrfs_free_space_entry {

	__le64 offset;

	__le64 bytes;

	u8 type;
} __attribute__ ((__packed__));

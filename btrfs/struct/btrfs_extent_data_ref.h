#pragma once

#include "../types.h"


struct btrfs_extent_data_ref {

    /* the objectid for the file tree that references this extent */
	__le64 root;

    /* the objectid of the inode that contains the EXTENT_DATA item that references this extent */
	__le64 objectid;

    /* the offset within the file that corresponds to this extent */
	__le64 offset;

    /* the number of references held */
	__le32 count;
} __attribute__ ((__packed__));

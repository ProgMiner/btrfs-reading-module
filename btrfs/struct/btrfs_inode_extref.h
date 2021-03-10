#pragma once

#include "../types.h"


struct btrfs_inode_extref {

	__le64 parent_objectid;

	__le64 index;

	__le16 name_len;

    /* name goes here */
	__u8 name[0];
} __attribute__ ((__packed__));

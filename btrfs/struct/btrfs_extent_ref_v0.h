#pragma once

#include "../types.h"


struct btrfs_extent_ref_v0 {

	__le64 root;

	__le64 generation;

	__le64 objectid;

	__le32 count;
} __attribute__ ((__packed__));

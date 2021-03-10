#pragma once

#include "../types.h"


struct btrfs_qgroup_info_item {

    /* id of transaction that modified this qgroup item */
	__le64 generation;

    /* referenced bytes (including shared data) */
	__le64 referenced;

    /* referenced bytes compressed */
	__le64 referenced_compressed;

    /* exclusive data referenced */
	__le64 exclusive;

    /* exclusive compressed bytes */
	__le64 exclusive_compressed;
} __attribute__ ((__packed__));

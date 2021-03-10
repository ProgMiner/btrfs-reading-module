#pragma once

#include "../types.h"


/*
 * this is used for both forward and backward root refs
 */
struct btrfs_root_ref {

    /* subtree ID */
	__le64 dirid;

    /* directory sequence number of subtree entry */
	__le64 sequence;

    /* length of subtree name */
	__le16 name_len;
} __attribute__ ((__packed__));

#pragma once

#include "../types.h"


#define BTRFS_TREE_BLOCK_REF_KEY   176
#define BTRFS_SHARED_BLOCK_REF_KEY 182
#define BTRFS_EXTENT_DATA_REF_KEY  178
#define BTRFS_SHARED_DATA_REF_KEY  184


struct btrfs_extent_inline_ref {

    /* type
     *
     * Types:
     *   - BTRFS_TREE_BLOCK_REF_KEY [176]
     *     The back reference is indirect for a tree block.
     *     offset contains the objectid of the tree root that allocated the block.
     *   - BTRFS_SHARED_BLOCK_REF_KEY [182]
     *     The back reference is shared for a tree block.
     *     offset contains the byte offset of the node at the next higher level
     *     in the tree where this block is located.
     *   - BTRFS_EXTENT_DATA_REF_KEY [178]
     *     The back reference is indirect for a data extent.
     *     offset is unused and a btrfs_extent_data_ref structure is located immediately
     *     after the type field.
     *   - BTRFS_SHARED_DATA_REF_KEY [184]
     *     The back reference is shared for a data extent.
     *     offset contains the byte offset of the metadata block that contains the EXTENT_DATA item
     *     that describes this extent. Immediately following is a btrfs_shared_data_ref structure
     *     containing the reference count.
     */
	u8 type;

    /* contents depend on type of reference */
	__le64 offset;
} __attribute__ ((__packed__));

#pragma once

#include "../types.h"


struct btrfs_dev_extent {

    /* objectid of the CHUNK_TREE that owns this extent. Always BTRFS_CHUNK_TREE_OBJECTID */
    __le64 chunk_tree;

    /* objectid of the CHUNK_ITEM that references this extent.
     * In practice, it will always be BTRFS_FIRST_CHUNK_TREE_OBJECTID
     */
    __le64 chunk_objectid;

    /* offset of the CHUNK_ITEM that references this extent */
    __le64 chunk_offset;

    /* length of this extent, in bytes */
    __le64 length;

    /* UUID of the CHUNK_TREE that owns this extent */
    __u8[16] chunk_tree_uuid;
} __attribute__ ((__packed__));

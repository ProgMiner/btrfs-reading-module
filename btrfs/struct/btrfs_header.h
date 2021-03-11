#pragma once

#include "../types.h"


/*
 * every tree block (leaf or node) starts with this header.
 */
struct btrfs_header {

    /* these first four must match the super block */
    u8 csum[BTRFS_CSUM_SIZE];

    /* FS specific uuid */
    u8 fsid[BTRFS_FSID_SIZE];

    /* virtual address of block
     *
     * which block this node is supposed to live in
     */
    __le64 bytenr;

    __le64 flags;

    /* allowed to be different from the super from here on down */
    u8 chunk_tree_uuid[BTRFS_UUID_SIZE];

    __le64 generation;

    /* the object id of the tree this block belongs to, for example BTRFS_ROOT_TREE_OBJECTID */
    __le64 owner;

    __le32 nritems;

    u8 level;
} __attribute__ ((__packed__));

static inline u8 btrfs_header_level(struct btrfs_header * header) {
    return le8_to_cpu(header->level);
}

static inline u32 btrfs_header_nritems(struct btrfs_header * header) {
    return le32_to_cpu(header->nritems);
}

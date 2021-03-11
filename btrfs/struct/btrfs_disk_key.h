#pragma once

#include "../types.h"
#include "btrfs_key.h"


/*
 * inode items have the data typically returned from stat and store other
 * info about object characteristics.  There is one for every file and dir in
 * the FS
 */
#define BTRFS_INODE_ITEM_KEY   1
#define BTRFS_INODE_REF_KEY    12
#define BTRFS_INODE_EXTREF_KEY 13
#define BTRFS_XATTR_ITEM_KEY   24
#define BTRFS_ORPHAN_ITEM_KEY  48

#define BTRFS_DIR_LOG_ITEM_KEY  60
#define BTRFS_DIR_LOG_INDEX_KEY 72
/*
 * dir items are the name -> inode pointers in a directory.  There is one
 * for every name in a directory.
 */
#define BTRFS_DIR_ITEM_KEY  84
#define BTRFS_DIR_INDEX_KEY 96

/*
 * extent data is for file data
 */
#define BTRFS_EXTENT_DATA_KEY 108

/*
 * csum items have the checksums for data in the extents
 */
#define BTRFS_CSUM_ITEM_KEY 120
/*
 * extent csums are stored in a separate tree and hold csums for
 * an entire extent on disk.
 */
#define BTRFS_EXTENT_CSUM_KEY 128

/*
 * root items point to tree roots.  There are typically in the root
 * tree used by the super block to find all the other trees
 */
#define BTRFS_ROOT_ITEM_KEY 132

/*
 * root backrefs tie subvols and snapshots to the directory entries that
 * reference them
 */
#define BTRFS_ROOT_BACKREF_KEY 144

/*
 * root refs make a fast index for listing all of the snapshots and
 * subvolumes referenced by a given root.  They point directly to the
 * directory item in the root that references the subvol
 */
#define BTRFS_ROOT_REF_KEY 156

/*
 * extent items are in the extent map tree.  These record which blocks
 * are used, and how many references there are to each block
 */
#define BTRFS_EXTENT_ITEM_KEY 168

/*
 * The same as the BTRFS_EXTENT_ITEM_KEY, except it's metadata we already know
 * the length, so we save the level in key->offset instead of the length.
 */
#define BTRFS_METADATA_ITEM_KEY 169

#define BTRFS_TREE_BLOCK_REF_KEY 176

#define BTRFS_EXTENT_DATA_REF_KEY 178

/* old style extent backrefs */
#define BTRFS_EXTENT_REF_V0_KEY 180

#define BTRFS_SHARED_BLOCK_REF_KEY 182

#define BTRFS_SHARED_DATA_REF_KEY 184


/*
 * block groups give us hints into the extent allocation trees.  Which
 * blocks are free etc etc
 */
#define BTRFS_BLOCK_GROUP_ITEM_KEY 192

/*
 * Every block group is represented in the free space tree by a free space info
 * item, which stores some accounting information. It is keyed on
 * (block_group_start, FREE_SPACE_INFO, block_group_length).
 */
#define BTRFS_FREE_SPACE_INFO_KEY 198

/*
 * A free space extent tracks an extent of space that is free in a block group.
 * It is keyed on (start, FREE_SPACE_EXTENT, length).
 */
#define BTRFS_FREE_SPACE_EXTENT_KEY 199

/*
 * When a block group becomes very fragmented, we convert it to use bitmaps
 * instead of extents. A free space bitmap is keyed on
 * (start, FREE_SPACE_BITMAP, length); the corresponding item is a bitmap with
 * (length / sectorsize) bits.
 */
#define BTRFS_FREE_SPACE_BITMAP_KEY 200

#define BTRFS_DEV_EXTENT_KEY 204
#define BTRFS_DEV_ITEM_KEY   216
#define BTRFS_CHUNK_ITEM_KEY 228

#define BTRFS_BALANCE_ITEM_KEY 248

/*
 * quota groups
 */
#define BTRFS_QGROUP_STATUS_KEY   240
#define BTRFS_QGROUP_INFO_KEY     242
#define BTRFS_QGROUP_LIMIT_KEY    244
#define BTRFS_QGROUP_RELATION_KEY 246

/*
 * Obsolete name, see BTRFS_TEMPORARY_ITEM_KEY.
 */
#define BTRFS_BALANCE_ITEM_KEY 248

/*
 * The key type for tree items that are stored persistently, but do not need to
 * exist for extended period of time. The items can exist in any tree.
 *
 * [subtype, BTRFS_TEMPORARY_ITEM_KEY, data]
 *
 * Existing items:
 *
 * - balance status item
 *   (BTRFS_BALANCE_OBJECTID, BTRFS_TEMPORARY_ITEM_KEY, 0)
 */
#define BTRFS_TEMPORARY_ITEM_KEY 248

/*
 * Obsolete name, see BTRFS_PERSISTENT_ITEM_KEY
 */
#define BTRFS_DEV_STATS_KEY 249

/*
 * The key type for tree items that are stored persistently and usually exist
 * for a long period, eg. filesystem lifetime. The item kinds can be status
 * information, stats or preference values. The item can exist in any tree.
 *
 * [subtype, BTRFS_PERSISTENT_ITEM_KEY, data]
 *
 * Existing items:
 *
 * - device statistics, store IO stats in the device tree, one key for all
 *   stats
 *   (BTRFS_DEV_STATS_OBJECTID, BTRFS_DEV_STATS_KEY, 0)
 */
#define BTRFS_PERSISTENT_ITEM_KEY 249

/*
 * Persistently stores the device replace state in the device tree.
 * The key is built like this: (0, BTRFS_DEV_REPLACE_KEY, 0).
 */
#define BTRFS_DEV_REPLACE_KEY 250


/*
 * The btrfs_key is one of the fundamental btrfs data structures.
 * Every item in every tree in the file system is located using its key.
 * The btrfs_key can be more accurately described as a 3-tuple used to
 * locate any item in any tree in the file system.
 *
 * btrfs_key objects only exists in memory and is in CPU byte order.
 * btrfs_disk_key is identical to btrfs_key except that objectid and offset are in
 * little endian (disk) byte order and are part of the file system on-disk format.
 */
struct btrfs_disk_key {

    /* the object identifier for this item
     *
     * The objectid field contains a value that is defined by the tree in which it is located.
     * For example, in the EXTENT_TREE, the objectid contains the starting byte offset
     * of the extent it describes.
     */
    __le64 objectid;

    /* the type of the item this key describes
     *
     * The type field is universal. It always contains the type of the item identified by the key.
     * There are no changes in usage or context that depend on the tree using the key.
     */
    u8 type;

    /* more accurately described as "third component."
     * It is literally an offset only in some contexts
     *
     * The offset field contains a value that is defined by the item type. For example,
     * keys of type = EXTENT_ITEM use the offset to store the size of the extent the item describes.
     * Keys of type = METADATA_ITEM use the offset to store the level
     * of the tree where the tree block is located.
     */
    __le64 offset;
} __attribute__ ((__packed__));

static inline void btrfs_disk_key_to_cpu(struct btrfs_key * cpu, struct btrfs_disk_key * disk) {
    cpu->offset = le64_to_cpu(disk->offset);
    cpu->type = disk->type;
    cpu->objectid = le64_to_cpu(disk->objectid);
}

#pragma once

#include "../types.h"
#include "btrfs_key.h"


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

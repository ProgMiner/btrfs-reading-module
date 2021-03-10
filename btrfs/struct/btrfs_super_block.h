#pragma once

#include "../types.h"
#include "../btrfs_consts.h"
#include "btrfs_dev_item.h"
#include "btrfs_root_backup.h"


/*
 * The primary superblock is located at 0x1 0000 (6410 KiB). Mirror copies of the superblock
 * are located at physical addresses 0x400 0000 (6410 MiB), 0x40 0000 0000 (25610 GiB),
 * and 0x4 0000 0000 0000 (1 PiB), if these locations are valid. btrfs normally updates
 * all superblocks, but in SSD mode it will update only one at a time.
 * The superblock with the highest generation is used when reading.
 *
 * Note that btrfs only recognizes disks with a valid 0x1 0000 superblock;
 * otherwise, there would be confusion with other filesystems.
 *
 *
 * https://btrfs.wiki.kernel.org/index.php/Data_Structures#btrfs_super_block
 */
struct btrfs_super_block {

	u8 csum[BTRFS_CSUM_SIZE];

	/* the first 3 fields must match struct btrfs_header */

    /* FS specific uuid */
	u8 fsid[BTRFS_FSID_SIZE];

    /* this block number */
	__le64 bytenr;

	__le64 flags;

	/* allowed to be different from the btrfs_header from here own down */

	__le64 magic;

	__le64 generation;

	__le64 root;

	__le64 chunk_root;

	__le64 log_root;

	/* this will help find the new super based on the log root */
	__le64 log_root_transid;

	__le64 total_bytes;

	__le64 bytes_used;

	__le64 root_dir_objectid;

	__le64 num_devices;

	__le32 sectorsize;

	__le32 nodesize;

	/* unused and must be equal to nodesize */
	__le32 __unused_leafsize;

	__le32 stripesize;

	__le32 sys_chunk_array_size;

	__le64 chunk_root_generation;

	__le64 compat_flags;

	__le64 compat_ro_flags;

	__le64 incompat_flags;

	__le16 csum_type;

	u8 root_level;

	u8 chunk_root_level;

	u8 log_root_level;

	struct btrfs_dev_item dev_item;

	char label[BTRFS_LABEL_SIZE];

	__le64 cache_generation;

	__le64 uuid_tree_generation;

	u8 metadata_uuid[BTRFS_FSID_SIZE];

	/* future expansion */
	__le64 reserved[28];

	u8 sys_chunk_array[BTRFS_SYSTEM_CHUNK_ARRAY_SIZE];

	struct btrfs_root_backup super_roots[BTRFS_NUM_BACKUP_ROOTS];
} __attribute__ ((__packed__));

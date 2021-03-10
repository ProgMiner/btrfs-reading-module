#pragma once

#include "../types.h"


#define BTRFS_ROOT_SUBVOL_RDONLY 0x1


struct btrfs_root_item {

	struct btrfs_inode_item inode;

	__le64 generation;

	__le64 root_dirid;

	__le64 bytenr;

	__le64 byte_limit;

	__le64 bytes_used;

	__le64 last_snapshot;

    /*
     *   - BTRFS_ROOT_SUBVOL_RDONLY [0x1]
     *     Flag to indicate that this root is read-only.
     */
	__le64 flags;

	__le32 refs;

	struct btrfs_disk_key drop_progress;

	u8 drop_level;

	u8 level;

	/*
	 * The following fields appear after subvol_uuids+subvol_times
	 * were introduced.
	 */

	/*
	 * This generation number is used to test if the new fields are valid
	 * and up to date while reading the root item. Every time the root item
	 * is written out, the "generation" field is copied into this field. If
	 * anyone ever mounted the fs with an older kernel, we will have
	 * mismatching generation values here and thus must invalidate the
	 * new fields. See btrfs_update_root and btrfs_find_last_root for
	 * details.
	 * the offset of generation_v2 is also used as the start for the memset
	 * when invalidating the fields.
	 */
	__le64 generation_v2;

	u8 uuid[BTRFS_UUID_SIZE];

	u8 parent_uuid[BTRFS_UUID_SIZE];

	u8 received_uuid[BTRFS_UUID_SIZE];

    /* updated when an inode changes */
	__le64 ctransid;

    /* trans when created */
	__le64 otransid;

    /* trans when sent. non-zero for received subvol */
	__le64 stransid;

    /* trans when received. non-zero for received subvol */
	__le64 rtransid;

	struct btrfs_timespec ctime;

	struct btrfs_timespec otime;

	struct btrfs_timespec stime;

	struct btrfs_timespec rtime;

    /* for future */
    __le64 reserved[8];
} __attribute__ ((__packed__));

#pragma once


/* 32 bytes in various csum fields */
#define BTRFS_CSUM_SIZE 32

#define BTRFS_FSID_SIZE 16
#define BTRFS_UUID_SIZE 16

/*
 * this is a very generous portion of the super block, giving us
 * room to translate 14 chunks with 3 stripes each.
 */
#define BTRFS_SYSTEM_CHUNK_ARRAY_SIZE 2048
#define BTRFS_LABEL_SIZE 256

/*
 * just in case we somehow lose the roots and are not able to mount,
 * we store an array of the roots from previous transactions
 * in the super.
 */
#define BTRFS_NUM_BACKUP_ROOTS 4

/* 64 KiB */
#define BTRFS_SUPER_INFO_OFFSET 0x00010000

#pragma once

#include <stdint.h>

#include <linux/types.h>


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


typedef uint8_t u8;

struct btrfs_dev_item {
    /* the internal btrfs device id */
    __le64 devid;

    /* size of the device */
    __le64 total_bytes;

    /* bytes used */
    __le64 bytes_used;

    /* optimal io alignment for this device */
    __le32 io_align;

    /* optimal io width for this device */
    __le32 io_width;

    /* minimal io size for this device */
    __le32 sector_size;

    /* type and info about this device */
    __le64 type;

    /* expected generation for this device */
    __le64 generation;

    /*
     * starting byte of this partition on the device,
     * to allow for stripe alignment in the future
     */
    __le64 start_offset;

    /* grouping information for allocation decisions */
    __le32 dev_group;

    /* seek speed 0-100 where 100 is fastest */
    u8 seek_speed;

    /* bandwidth 0-100 where 100 is fastest */
    u8 bandwidth;

    /* btrfs generated uuid for this device */
    u8 uuid[BTRFS_UUID_SIZE];

    /* uuid of FS who owns this device */
    u8 fsid[BTRFS_UUID_SIZE];
} __attribute__ ((__packed__));

/*
 * just in case we somehow lose the roots and are not able to mount,
 * we store an array of the roots from previous transactions
 * in the super.
 */
#define BTRFS_NUM_BACKUP_ROOTS 4
struct btrfs_root_backup {
    __le64 tree_root;
    __le64 tree_root_gen;

    __le64 chunk_root;
    __le64 chunk_root_gen;

    __le64 extent_root;
    __le64 extent_root_gen;

    __le64 fs_root;
    __le64 fs_root_gen;

    __le64 dev_root;
    __le64 dev_root_gen;

    __le64 csum_root;
    __le64 csum_root_gen;

    __le64 total_bytes;
    __le64 bytes_used;
    __le64 num_devices;
    __le64 unsed_64[4]; /* future */

    u8 tree_root_level;
    u8 chunk_root_level;
    u8 extent_root_level;
    u8 fs_root_level;
    u8 dev_root_level;
    u8 csum_root_level;
    u8 unused_8[10];    /* future and to align */
} __attribute__ ((__packed__));

/* super block struct */
struct btrfs_super_block {
    u8 csum[BTRFS_CSUM_SIZE];   /* checksum */

    /* the first 3 fields must match struct btrfs_header */
    u8 fsid[BTRFS_FSID_SIZE];   /* FS specific uuid */
    __le64 bytenr;              /* this block number */
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
    __le32 __unused_leafsize;   /* unused and must be equal to nodesize */
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
    __le64 reserved[28];    /* future expansion */
    u8 sys_chunk_array[BTRFS_SYSTEM_CHUNK_ARRAY_SIZE];
    struct btrfs_root_backup super_roots[BTRFS_NUM_BACKUP_ROOTS];
} __attribute__ ((__packed__));


struct btrfs_super_block * btrfs_low_find_superblock(void * data);

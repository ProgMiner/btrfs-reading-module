#pragma once

#include "../types.h"
#include "../btrfs_consts.h"
#include "btrfs_dev_item.h"
#include "btrfs_root_backup.h"


/* ascii _BHRfS_M, no null */
#define BTRFS_MAGIC 0x4D5F53665248425FULL

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

static inline u32 btrfs_super_block_sys_chunk_array_size(
        const struct btrfs_super_block * super_block
) {
    return le32_to_cpu(super_block->sys_chunk_array_size);
}

static inline u64 btrfs_super_block_chunk_root(const struct btrfs_super_block * super_block) {
    return le64_to_cpu(super_block->chunk_root);
}

static inline u64 btrfs_super_block_root(const struct btrfs_super_block * super_block) {
    return le64_to_cpu(super_block->root);
}

static inline u64 btrfs_super_block_bytenr(const struct btrfs_super_block * super_block) {
    return le64_to_cpu(super_block->bytenr);
}

static inline u64 btrfs_super_block_magic(const struct btrfs_super_block * super_block) {
    return le64_to_cpu(super_block->magic);
}

#pragma once

#include <stdint.h>
#include <byteswap.h>


#define __force
#define __bitwise

/* Macros to generate set/get funcs for the struct fields
 * assume there is a lefoo_to_cpu for every type, so lets make a simple
 * one for u8:
 */
#define le8_to_cpu(v) (v)
#define cpu_to_le8(v) (v)
#define __le8 u8

#if __BYTE_ORDER == __BIG_ENDIAN
#define cpu_to_le64(x) ((__force __le64)(u64)(bswap_64(x)))
#define le64_to_cpu(x) ((__force u64)(__le64)(bswap_64(x)))
#define cpu_to_le32(x) ((__force __le32)(u32)(bswap_32(x)))
#define le32_to_cpu(x) ((__force u32)(__le32)(bswap_32(x)))
#define cpu_to_le16(x) ((__force __le16)(u16)(bswap_16(x)))
#define le16_to_cpu(x) ((__force u16)(__le16)(bswap_16(x)))
#else
#define cpu_to_le64(x) ((__force __le64)(u64)(x))
#define le64_to_cpu(x) ((__force u64)(__le64)(x))
#define cpu_to_le32(x) ((__force __le32)(u32)(x))
#define le32_to_cpu(x) ((__force u32)(__le32)(x))
#define cpu_to_le16(x) ((__force __le16)(u16)(x))
#define le16_to_cpu(x) ((__force u16)(__le16)(x))
#endif


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


typedef int8_t  s8;
typedef uint8_t u8;
typedef int16_t  s16;
typedef uint16_t u16;
typedef int32_t  s32;
typedef uint32_t u32;
typedef int64_t  s64;
typedef uint64_t u64;

typedef u16 __bitwise __le16;
typedef u16 __bitwise __be16;
typedef u32 __bitwise __le32;
typedef u32 __bitwise __be32;
typedef u64 __bitwise __le64;
typedef u64 __bitwise __be64;


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

/*
 * the key defines the order in the tree, and so it also defines (optimal)
 * block layout.  objectid corresponds to the inode number.  The flags
 * tells us things about the object, and is a kind of stream selector.
 * so for a given inode, keys with flags of 1 might refer to the inode
 * data, flags of 2 may point to file data in the btree and flags == 3
 * may point to extents.
 *
 * offset is the starting byte offset for this key in the stream.
 *
 * btrfs_disk_key is in disk byte order.  struct btrfs_key is always
 * in cpu native order.  Otherwise they are identical and their sizes
 * should be the same (ie both packed)
 */
struct btrfs_disk_key {
	__le64 objectid;
	u8 type;
	__le64 offset;
} __attribute__ ((__packed__));

struct btrfs_key {
	u64 objectid;
	u8 type;
	u64 offset;
} __attribute__ ((__packed__));


static inline void btrfs_disk_key_to_cpu(struct btrfs_key * cpu, struct btrfs_disk_key * disk) {
    cpu->offset = le64_to_cpu(disk->offset);
    cpu->type = disk->type;
    cpu->objectid = le64_to_cpu(disk->objectid);
}


struct btrfs_super_block * btrfs_low_find_superblock(void * data);

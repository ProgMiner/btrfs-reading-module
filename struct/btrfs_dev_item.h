#pragma once

#include "../types.h"


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

    /* starting byte of this partition on the device, to allow for stripe alignment in the future */
    __le64 start_offset;

    /* grouping information for allocation decisions */
    __le32 dev_group;

    /* seek speed 0-100 where 100 is fastest */
    __u8 seek_speed;

    /* bandwidth 0-100 where 100 is fastest */
    __u8 bandwidth;

    /* btrfs generated uuid for this device */
    __u8 uuid[16];

    /* uuid of FS who owns this device */
    __u8 fsid[16];
} __attribute__ ((__packed__));

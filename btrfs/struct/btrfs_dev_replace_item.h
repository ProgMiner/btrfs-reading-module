#pragma once

#include "../types.h"


struct btrfs_dev_replace_item {

    /* device id of the source device */
    __le64 src_devid;

    __le64 cursor_left;

    __le64 cursor_right;

    __le64 cont_reading_from_srcdev_mode;

    /* state of replace operation. Can be one of:
     * BTRFS_IOCTL_DEV_REPLACE_STATE_STARTED,
     * BTRFS_IOCTL_DEV_REPLACE_STATE_STARTED,
     * BTRFS_IOCTL_DEV_REPLACE_STATE_STARTED,
     * BTRFS_IOCTL_DEV_REPLACE_STATE_STARTED,
     * BTRFS_IOCTL_DEV_REPLACE_STATE_STARTED
     */
    __le64 replace_state;

    __le64 time_started;

    __le64 time_stopped;

    /* expected generation for this device */
    __le64 num_write_errors;

    __le64 num_uncorrectable_read_errors;
} __attribute__ ((__packed__));

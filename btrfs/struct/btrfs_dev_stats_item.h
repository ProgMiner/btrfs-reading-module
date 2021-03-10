#pragma once

#include "../types.h"


#define BTRFS_DEV_STAT_WRITE_ERRS 0x1
#define BTRFS_DEV_STAT_READ_ERRS  0x2
#define BTRFS_DEV_STAT_FLUSH_ERRS 0x3

#define BTRFS_DEV_STAT_CORRUPTION_ERRS 0x4
#define BTRFS_DEV_STAT_GENERATION_ERRS 0x5


struct btrfs_dev_stats_item {

    /* array of stats values
     *
     * Stat items:
     *
     * The codes below signify a direct disk I/O failures:
     * 
     *   - BTRFS_DEV_STAT_WRITE_ERRS 0x1
     *     Number of times we got EIO or EREMOTEIO from lower layers while performing writes
     *   - BTRFS_DEV_STAT_READ_ERRS 0x2
     *     Number of times we got EIO or EREMOTEIO from lower layers while performing reads
     *   - BTRFS_DEV_STAT_FLUSH_ERRS 0x3
     *     Number of times we got EIO or EREMOTEIO from lower layers while performing data flushes
     *
     * The codes below indicate an indirect I/O failure:
     *
     *   - BTRFS_DEV_STAT_CORRUPTION_ERRS 0x4
     *     checksum error, bytenr error or contents is illegal:
     *     this is an indication that the block was damaged during read or write,
     *     or written to wrong location or read from wrong location
     *   - BTRFS_DEV_STAT_GENERATION_ERRS 0x5
     *     an indication that blocks have not been written
     */
    __le64 values[6];
} __attribute__ ((__packed__));

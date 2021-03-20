#pragma once

#include "../types.h"


#define BTRFS_QGROUP_STATUS_VERSION 1

#define BTRFS_QGROUP_STATUS_FLAG_ON           0x1
#define BTRFS_QGROUP_STATUS_FLAG_RESCAN       0x2
#define BTRFS_QGROUP_STATUS_FLAG_INCONSISTENT 0x4


/*
 * One item per quota tree. Holds information about quota state.
 */
struct btrfs_qgroup_status_item {

    /* quota version. Currently always set to 1 (BTRFS_QGROUP_STATUS_VERSION) */
    __le64 version;

    /* number updated during every commit. Used to check for inconsistencies */
    __le64 generation;

    /* flags
     *
     * Flags:
     *   - BTRFS_QGROUP_STATUS_FLAG_ON [0x1]
     *     subvolume quota turned on.
     *   - BTRFS_QGROUP_STATUS_FLAG_RESCAN [0x2]
     *     Signals whether rescan is in progress.
     *   - BTRFS_QGROUP_STATUS_FLAG_INCONSISTENT [0x4]
     *     Turned on when some qgroup entries are known to be out of date,
     *     either because the configuration has changed in a certain way that requires rescan
     *     or because the fs has been mounted with a non-qgroup-aware version.
     *     Turning qouta off and on again makes it inconsistent, too.
     */
    __le64 flags;

    /* used during rescan to record the progress. It contains a logical address
     *
     * progress during scanning
     */
    __le64 rescan;
} __attribute__ ((__packed__));

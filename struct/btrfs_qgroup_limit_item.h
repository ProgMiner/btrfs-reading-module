#pragma once


#define BTRFS_QGROUP_LIMIT_MAX_RFER  (1ULL << 0)
#define BTRFS_QGROUP_LIMIT_MAX_EXCL  (1ULL << 1)
#define BTRFS_QGROUP_LIMIT_RSV_RFER  (1ULL << 2)
#define BTRFS_QGROUP_LIMIT_RSV_EXCL  (1ULL << 3)
#define BTRFS_QGROUP_LIMIT_RFER_CMPR (1ULL << 4)
#define BTRFS_QGROUP_LIMIT_EXCL_CMPR (1ULL << 5)


struct btrfs_qgroup_limit_item {

    /* contains information about enabled limits
     *
     * Flags
     *   - BTRFS_QGROUP_LIMIT_MAX_RFER [0x1]
     *     Limit on maximum referenced data enabled
     *   - BTRFS_QGROUP_LIMIT_MAX_EXCL [0x2]
     *     Limit on maximum eclusive data enabled
     *   - BTRFS_QGROUP_LIMIT_RSV_RFER [0x4]
     *     Unused
     *   - BTRFS_QGROUP_LIMIT_RSV_EXCL [0x8]
     *     Unused
     *   - BTRFS_QGROUP_LIMIT_RFER_CMPR [0x10]
     *     Limit on maximum compressed referenced data enabled
     *   - BTRFS_QGROUP_LIMIT_EXCL_CMPR [0x20]
     *     Limit on maximum exclusive compressed data size enabled
     */
    __le64 flags;

    /* maximum amount of referenced data allowed */
    __le64 max_referenced;

    /* maximum amount of exclusive data allowed */
    __le64 max_exclusive;

    /* unused */
    __le64 rsv_referenced;

    /* unused */
    __le64 rsv_exclusive;
} __attribute__ ((__packed__));

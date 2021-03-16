#pragma once

#include "../types.h"


struct btrfs_timespec {

    __le64 sec;

    __le32 nsec;
} __attribute__ ((__packed__));

static inline u64 btrfs_timespec_sec(struct btrfs_timespec * timespec) {
    return le64_to_cpu(timespec->sec);
}

static inline u32 btrfs_timespec_nsec(struct btrfs_timespec * timespec) {
    return le32_to_cpu(timespec->nsec);
}

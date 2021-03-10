#pragma once

#include "../types.h"


struct btrfs_timespec {

	__le64 sec;

	__le32 nsec;
} __attribute__ ((__packed__));

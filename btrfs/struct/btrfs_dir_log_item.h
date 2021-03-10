#pragma once

#include "../types.h"


struct btrfs_dir_log_item {

	__le64 end;
} __attribute__ ((__packed__));

#pragma once


#ifdef BTRFS_DEBUG
#include <stdio.h>

#define btrfs_debug_printf(__format, ...) printf(__format, ##__VA_ARGS__)
#else
#define btrfs_debug_printf(__format, ...)
#endif

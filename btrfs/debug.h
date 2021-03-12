#pragma once


#ifdef BTRFS_DEBUG
#include <stdio.h>

#define btrfs_debug_printf(...) printf(__VA_ARGS__)
#else
#define btrfs_debug_printf(...)
#endif

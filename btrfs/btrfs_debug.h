#pragma once


#ifdef BTRFS_DEBUG
#include <stdio.h>

#include "types.h"


#define btrfs_debug_printf(...) printf(__VA_ARGS__)


void btrfs_debug_increase_indent(u8 shift);
void btrfs_debug_decrease_indent(u8 shift);
void btrfs_debug_indent();

#else
#define btrfs_debug_printf(...)
#define btrfs_debug_increase_indent(__shift)
#define btrfs_debug_decrease_indent(__shift)
#define btrfs_debug_indent()
#endif

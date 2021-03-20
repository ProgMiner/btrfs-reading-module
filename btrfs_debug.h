#pragma once


#ifdef BTRFS_DEBUG
#include <stdio.h>

#include "types.h"


#define btrfs_debug_printf(...) fprintf(stderr, __VA_ARGS__)
#define btrfs_debug_start_section(__name) { \
    btrfs_debug_indent(); \
    btrfs_debug_printf("----  " __name " ----\n"); \
    btrfs_debug_increase_indent(1); \
}
#define btrfs_debug_end_section(__name) { \
    btrfs_debug_decrease_indent(1); \
    btrfs_debug_indent(); \
    btrfs_debug_printf("---- /" __name " ----\n"); \
}


void btrfs_debug_increase_indent(u8 shift);
void btrfs_debug_decrease_indent(u8 shift);
void btrfs_debug_indent();

#else
#define btrfs_debug_printf(...)
#define btrfs_debug_start_section(__name)
#define btrfs_debug_end_section(__name)
#define btrfs_debug_increase_indent(__shift)
#define btrfs_debug_decrease_indent(__shift)
#define btrfs_debug_indent()
#endif

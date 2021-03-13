#include "btrfs_debug.h"

#ifdef BTRFS_DEBUG
#include <string.h>


static u8 btrfs_debug_indentation = 0;

void btrfs_debug_increase_indent(u8 shift) {
    btrfs_debug_indentation += shift;
}

void btrfs_debug_decrease_indent(u8 shift) {
    btrfs_debug_indentation -= shift;
}

static char * __btrfs_debug_indent() {
    /* 256 spaces */
    static char * buffer =
            "                                                                "
            "                                                                "
            "                                                                "
            "                                                                ";
    return strdup(buffer);
}

void btrfs_debug_indent() {
    static char * buffer = NULL;

    if (!buffer) {
        buffer = __btrfs_debug_indent();
    }

    buffer[btrfs_debug_indentation] = '\0';
    printf("%s", buffer);
    buffer[btrfs_debug_indentation] = ' ';
}
#else
typedef int make_iso_compilers_happy;
#endif

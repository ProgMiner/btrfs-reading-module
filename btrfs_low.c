#include "btrfs_low.h"


struct btrfs_super_block * btrfs_low_find_superblock(void * data) {
    // TODO add checking for errors
    // TODO maybe add searching mirrors?

    return (void *) (((uint8_t *) data) + BTRFS_SUPER_INFO_OFFSET);
}

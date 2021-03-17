#include "btrfs_item.h"

#include "../btrfs_debug.h"
#include "btrfs_disk_key.h"


#ifdef BTRFS_DEBUG
void btrfs_item_print(const struct btrfs_item * item) {
    btrfs_disk_key_print(&item->key);

    btrfs_debug_indent();
    btrfs_debug_printf("offset: %u, size: %u\n", btrfs_item_offset(item), btrfs_item_size(item));
}
#endif

#include "btrfs_key_pointer.h"

#include "../btrfs_debug.h"
#include "btrfs_disk_key.h"


#ifdef BTRFS_DEBUG
void btrfs_key_pointer_print(const struct btrfs_key_pointer * key_pointer) {
    btrfs_disk_key_print(&key_pointer->key);

    btrfs_debug_indent();
    btrfs_debug_printf("blocknr: %llu, generation: %llu\n",
            btrfs_key_pointer_blocknr(key_pointer),
            btrfs_key_pointer_generation(key_pointer));
}
#endif

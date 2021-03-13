#include "btrfs_disk_key.h"


#ifdef BTRFS_DEBUG
void btrfs_disk_key_print(struct btrfs_disk_key * disk_key) {
    struct btrfs_key key;

    btrfs_disk_key_to_cpu(&key, disk_key);
    btrfs_key_print(&key);
}
#endif

#include "btrfs_low.h"

#include <string.h>
#include <stdlib.h>

#include "btrfs_find_in_btree.h"
#include "btrfs_traverse_btree.h"
#include "struct/btrfs_key_pointer.h"
#include "struct/btrfs_root_item.h"
#include "struct/btrfs_disk_key.h"
#include "struct/btrfs_header.h"
#include "struct/btrfs_chunk.h"
#include "struct/btrfs_item.h"


struct btrfs_super_block * btrfs_low_find_superblock(void * data) {
    /* TODO add checking for errors */
    /* TODO maybe add searching mirrors? */

    return (void *) (((uint8_t *) data) + BTRFS_SUPER_INFO_OFFSET);
}

struct btrfs_chunk_list * btrfs_low_read_sys_array(struct btrfs_super_block * sb) {
    u32 length = btrfs_super_block_sys_chunk_array_size(sb);
    u8 * position = sb->sys_chunk_array;
    struct btrfs_key key;

    struct btrfs_chunk_list * result = NULL, * prev_result;
    while (position - sb->sys_chunk_array < length) {
        btrfs_disk_key_to_cpu(&key, (struct btrfs_disk_key *) position);
        position += sizeof(struct btrfs_disk_key);

        prev_result = result;
        result = btrfs_chunk_list_new(key, (struct btrfs_chunk *) position, result);

        if (!result) {
            btrfs_chunk_list_delete(prev_result);
            return NULL;
        }

        position += btrfs_chunk_size((struct btrfs_chunk *) position);
    }

    return result;
}

static enum btrfs_traverse_btree_handler_result __btrfs_low_read_chunk_tree_handler(
        struct btrfs_chunk_list ** result,
        struct btrfs_key item_key,
        void * item_data
) {
    struct btrfs_chunk_list * new_result;

    if (item_key.type != BTRFS_CHUNK_ITEM_KEY) {
        btrfs_traverse_btree_continue;
    }

    if (!(new_result = btrfs_chunk_list_new(item_key, item_data, *result))) {
        btrfs_traverse_btree_break;
    }

    *result = new_result;
    btrfs_traverse_btree_continue;
}

struct btrfs_chunk_list * btrfs_low_read_chunk_tree(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 chunk_root
) {
    struct btrfs_chunk_list * result = chunk_list;

    btrfs_debug_printf("Reading chunk tree:\n");
    btrfs_traverse_btree(chunk_list, data, chunk_root, &result, __btrfs_low_read_chunk_tree_handler);

    return result;
}

u64 btrfs_low_find_root_fs_tree_root(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 root
) {
    struct btrfs_root_item * root_item;
    struct btrfs_key key;

    key.objectid = BTRFS_FS_TREE_OBJECTID;
    key.type = BTRFS_ROOT_ITEM_KEY;
    key.offset = 0;

    btrfs_debug_printf("Find root FS_TREE root:\n");
    root_item = btrfs_find_in_btree(chunk_list, data, root, key, NULL);

    return root_item ? root_item->bytenr : 0;
}

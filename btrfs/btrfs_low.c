#include "btrfs_low.h"

#include <string.h>
#include <stdlib.h>

#include "struct/btrfs_key_pointer.h"
#include "struct/btrfs_disk_key.h"
#include "struct/btrfs_header.h"
#include "struct/btrfs_chunk.h"
#include "struct/btrfs_item.h"


struct btrfs_super_block * btrfs_low_find_superblock(void * data) {
    /* TODO add checking for errors */
    /* TODO maybe add searching mirrors? */

    return (void *) (((uint8_t *) data) + BTRFS_SUPER_INFO_OFFSET);
}

struct btrfs_chunk_list * btrfs_read_sys_array(struct btrfs_super_block * sb) {
    u32 length = le32_to_cpu(sb->sys_chunk_array_size);
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

static enum btrfs_traverse_btree_handler_result __btrfs_traverse_btree_do(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 btree_root,
        void * acc,
        btrfs_traverse_btree_handler handler
) {
    struct btrfs_header * header = btrfs_chunk_list_resolve(chunk_list, data, btree_root);
    enum btrfs_traverse_btree_handler_result result;
    struct btrfs_key_pointer * key_ptr;
    struct btrfs_item * item;
    struct btrfs_key key_buf;
    u32 nritems, i;
    u8 level;

    if (!header) {
        return BTRFS_TRAVERSE_BTREE_CONTINUE;
    }

    nritems = btrfs_header_nritems(header);
    level = btrfs_header_level(header);

    if (level > 0) {
        key_ptr = (void *) (header + 1);

        for (i = 0; i < nritems; ++i, ++key_ptr) {
            result = __btrfs_traverse_btree_do(
                    chunk_list,
                    data,
                    btrfs_key_pointer_blocknr(key_ptr),
                    acc,
                    handler
            );

            if (result == BTRFS_TRAVERSE_BTREE_BREAK) {
                break;
            }
        }
    } else {
        item = (void *) (header + 1);

        for (i = 0; i < nritems; ++i, ++item) {
            btrfs_disk_key_to_cpu(&key_buf, &item->key);

            result = handler(
                    acc,
                    key_buf,
                    (void *) ((u8 *) (header + 1) + btrfs_item_offset(item))
            );

            if (result == BTRFS_TRAVERSE_BTREE_BREAK) {
                break;
            }
        }
    }

    return result;
}

void btrfs_traverse_btree_do(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 btree_root,
        void * acc,
        btrfs_traverse_btree_handler handler
) {
    __btrfs_traverse_btree_do(chunk_list, data, btree_root, acc, handler);
}

static enum btrfs_traverse_btree_handler_result __btrfs_read_chunk_tree_handler(
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

struct btrfs_chunk_list * btrfs_read_chunk_tree(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 chunk_root
) {
    struct btrfs_chunk_list * result = chunk_list;

    btrfs_traverse_btree(chunk_list, data, chunk_root, &result, __btrfs_read_chunk_tree_handler);

    return result;
}

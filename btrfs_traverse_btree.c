#include "btrfs_traverse_btree.h"

#include "struct/btrfs_key_pointer.h"
#include "struct/btrfs_disk_key.h"
#include "struct/btrfs_header.h"
#include "struct/btrfs_item.h"
#include "struct/btrfs_key.h"
#include "btrfs_debug.h"


static enum btrfs_traverse_btree_handler_result __btrfs_traverse_btree_do(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 btree_root,
        void * acc,
        btrfs_traverse_btree_handler handler
) {
    const struct btrfs_header * header = btrfs_chunk_list_resolve(chunk_list, data, btree_root);
    enum btrfs_traverse_btree_handler_result result = BTRFS_TRAVERSE_BTREE_CONTINUE;
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
            btrfs_debug_indent();
            btrfs_debug_printf("  - key_pointer:\n");

            btrfs_debug_increase_indent(4);
            btrfs_key_pointer_print(key_ptr);
            btrfs_debug_decrease_indent(4);

            btrfs_debug_increase_indent(2);
            result = __btrfs_traverse_btree_do(
                    chunk_list,
                    data,
                    btrfs_key_pointer_blocknr(key_ptr),
                    acc,
                    handler
            );
            btrfs_debug_decrease_indent(2);

            if (result == BTRFS_TRAVERSE_BTREE_BREAK) {
                break;
            }
        }
    } else {
        item = (void *) (header + 1);

        for (i = 0; i < nritems; ++i, ++item) {
            btrfs_disk_key_to_cpu(&key_buf, &item->key);

            btrfs_debug_indent();
            btrfs_debug_printf("  - item:\n");

            btrfs_debug_increase_indent(4);
            btrfs_item_print(item);
            btrfs_debug_decrease_indent(4);

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
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 btree_root,
        void * acc,
        btrfs_traverse_btree_handler handler
) {
    btrfs_debug_start_section("btrfs_traverse_btree");

    btrfs_debug_indent();
    btrfs_debug_printf("root:\n");
    __btrfs_traverse_btree_do(chunk_list, data, btree_root, acc, handler);

    btrfs_debug_end_section("btrfs_traverse_btree");
}

#ifdef BTRFS_DEBUG
static enum btrfs_traverse_btree_handler_result btrfs_traverse_btree_print_handler(
        void * acc,
        struct btrfs_key item_key,
        const void * item_data
) {
    btrfs_traverse_btree_continue;
}

void btrfs_traverse_btree_print(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 btree_root
) {
    btrfs_traverse_btree(chunk_list, data, btree_root, NULL, btrfs_traverse_btree_print_handler);
}
#endif

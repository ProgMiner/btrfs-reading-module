#include "btrfs_find_in_btree.h"

#include <stdint.h>

#include "struct/btrfs_key_pointer.h"
#include "struct/btrfs_disk_key.h"
#include "struct/btrfs_header.h"
#include "struct/btrfs_item.h"
#include "struct/btrfs_key.h"
#include "btrfs_debug.h"


static int btrfs_comp_keys(
        struct btrfs_key a,
        struct btrfs_key b
) {
	if (a.objectid > b.objectid) {
		return 1;
    }

	if (a.objectid < b.objectid) {
		return -1;
    }

	if (a.type > b.type) {
		return 1;
    }

	if (a.type < b.type) {
		return -1;
    }

	if (a.offset > b.offset) {
		return 1;
    }

	if (a.offset < b.offset) {
		return -1;
    }

	return 0;
}

/* binary search
 *
 * if found returns 0 and result is item with specified key
 * otherwise returns 1 and result is maximal item that key lower than query key
 */
static int __btrfs_binary_search(
        void * start,
        size_t count,
        off_t key_offset,
        size_t item_size,
        struct btrfs_key key,
        void ** result
) {
    size_t left = 0, right = count, middle;
    struct btrfs_key middle_key;
    int rel;

    btrfs_debug_indent();
    btrfs_debug_printf("----  btrfs_binary_search ----\n");

    btrfs_debug_indent();
    btrfs_debug_printf("Searching key:\n");
    btrfs_key_print(&key);

    while (left < right) {
        btrfs_debug_indent();
        btrfs_debug_printf("Current range: [%lu, %lu)\n", left, right);

        middle = left + (right - left) / 2;
        *result = (u8 *) start + middle * item_size;

        btrfs_disk_key_to_cpu(&middle_key, (void *) (((u8 *) *result) + key_offset));
        rel = btrfs_comp_keys(middle_key, key);

        btrfs_debug_indent();
        btrfs_debug_printf("Comparing with #%lu:\n", middle);
        btrfs_key_print(&middle_key);

        if (rel < 0) {
            left = middle + 1;
        } else if (rel > 0) {
            right = middle;
        } else {
            break;
        }
    }

    btrfs_debug_indent();
    btrfs_debug_printf("----  btrfs_binary_search ----\n");

    if (rel == 0) {
        return 0;
    } else {
        return 1;
    }
}

static int btrfs_binary_search(
        struct btrfs_header * node,
        struct btrfs_key key,
        void ** result
) {
    u32 nritems = btrfs_header_nritems(node);

    if (btrfs_header_level(node) > 0) {
        return __btrfs_binary_search(
                node + 1,
                nritems,
                offsetof(struct btrfs_key_pointer, key),
                sizeof(struct btrfs_key_pointer),
                key,
                result
        );
    } else {
        return __btrfs_binary_search(
                node + 1,
                nritems,
                offsetof(struct btrfs_item, key),
                sizeof(struct btrfs_item),
                key,
                result
        );
    }
}

void * btrfs_find_in_btree(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 btree_root,
        struct btrfs_key key,
        struct btrfs_key * result
) {
    struct btrfs_header * header = btrfs_chunk_list_resolve(chunk_list, data, btree_root);
    struct btrfs_key_pointer * key_ptr;
    struct btrfs_item * item;

    if (!header) {
        return NULL;
    }

    btrfs_debug_indent();
    btrfs_debug_printf("----  btrfs_find_in_btree ----\n");

    while (btrfs_header_level(header) > 0) {
        btrfs_binary_search(header, key, (void **) &key_ptr);

        btrfs_debug_indent();
        btrfs_debug_printf("  - key_pointer:\n");

        btrfs_debug_increase_indent(4);
        btrfs_key_pointer_print(key_ptr);
        btrfs_debug_decrease_indent(4);

        header = btrfs_chunk_list_resolve(chunk_list, data, btrfs_key_pointer_blocknr(key_ptr));
    }

    btrfs_binary_search(header, key, (void **) &item);

    btrfs_debug_indent();
    btrfs_debug_printf("  - item:\n");

    btrfs_debug_increase_indent(4);
    btrfs_item_print(item);
    btrfs_debug_decrease_indent(4);

    btrfs_debug_printf("---- /btrfs_find_in_btree ----\n");

    if (result) {
        btrfs_disk_key_to_cpu(result, &item->key);
    }

    return ((u8 *) (header + 1) + btrfs_item_offset(item));
}

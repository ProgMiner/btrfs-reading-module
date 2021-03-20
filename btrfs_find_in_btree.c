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
 * otherwise returns 1 and result is item with maximal key that lower than query key
 * if that item is not present, returns -1 and result is not defined
 */
static int __btrfs_binary_search(
        const void * start,
        size_t count,
        size_t key_offset,
        size_t item_size,
        struct btrfs_key key,
        const void ** result
) {
    size_t left = 0, right = count, middle;
    struct btrfs_key middle_key;
    const u8 * middle_item;
    int rel;

    if (right == left) {
        return -1;
    }

    btrfs_debug_start_section("btrfs_binary_search");

    btrfs_debug_indent();
    btrfs_debug_printf("Searching key:\n");
    btrfs_key_print(&key);

    while (left < right) {
        btrfs_debug_indent();
        btrfs_debug_printf("Current range: [%zu, %zu)\n", left, right);

        middle = left + (right - left) / 2;
        middle_item = (u8 *) start + middle * item_size;

        btrfs_disk_key_to_cpu(&middle_key, (void *) (middle_item + key_offset));
        rel = btrfs_comp_keys(middle_key, key);

        btrfs_debug_indent();
        btrfs_debug_printf("Comparing with #%zu:\n", middle);
        btrfs_key_print(&middle_key);

        if (rel < 0) {
            left = middle + 1;
        } else if (rel > 0) {
            right = middle;
        } else {
            break;
        }
    }

    btrfs_debug_end_section("btrfs_binary_search");

    if (rel == 0) {
        *result = (u8 *) start + middle * item_size;
        return 0;
    } else if (left == 0) {
        return -1;
    } else {
        *result = (u8 *) start + (left - 1) * item_size;
        return 1;
    }
}

static int btrfs_binary_search(
        const struct btrfs_header * node,
        struct btrfs_key key,
        const void ** result
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

const void * btrfs_find_in_btree(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 btree_root,
        struct btrfs_key key,
        struct btrfs_key * result,
        bool exact
) {
    const struct btrfs_header * header = btrfs_chunk_list_resolve(chunk_list, data, btree_root);
    const struct btrfs_key_pointer * key_ptr;
    const struct btrfs_item * item;
    bool found;
    int ret;

    if (!header) {
        return NULL;
    }

    btrfs_debug_start_section("btrfs_find_in_btree");

    btrfs_debug_indent();
    btrfs_debug_printf("Searching key:\n");
    btrfs_key_print(&key);

    while (btrfs_header_level(header) > 0) {
        btrfs_binary_search(header, key, (const void **) &key_ptr);

        btrfs_debug_indent();
        btrfs_debug_printf("  - level %u key_pointer:\n", btrfs_header_level(header));

        btrfs_debug_increase_indent(4);
        btrfs_key_pointer_print(key_ptr);
        btrfs_debug_decrease_indent(4);

        header = btrfs_chunk_list_resolve(chunk_list, data, btrfs_key_pointer_blocknr(key_ptr));
    }

    ret = btrfs_binary_search(header, key, (const void **) &item);
    found = ret == 0 || (ret > 0 && !exact);

    if (found) {
        btrfs_debug_indent();
        btrfs_debug_printf("  - item:\n");

        btrfs_debug_increase_indent(4);
        btrfs_item_print(item);
        btrfs_debug_decrease_indent(4);
    } else {
        btrfs_debug_indent();
        btrfs_debug_printf("item not found\n");
    }

    btrfs_debug_end_section("btrfs_find_in_btree");

    if (!found) {
        return 0;
    }

    if (result) {
        btrfs_disk_key_to_cpu(result, &item->key);
    }

    return ((u8 *) (header + 1) + btrfs_item_offset(item));
}

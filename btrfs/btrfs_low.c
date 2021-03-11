#include "btrfs_low.h"

#include <string.h>
#include <stdlib.h>

#include "struct/btrfs_key_pointer.h"
#include "struct/btrfs_disk_key.h"
#include "struct/btrfs_header.h"
#include "struct/btrfs_chunk.h"
#include "struct/btrfs_item.h"


struct btrfs_super_block * btrfs_low_find_superblock(void * data) {
    // TODO add checking for errors
    // TODO maybe add searching mirrors?

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

struct btrfs_chunk_list * btrfs_read_chunk_tree(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 chunk_root
) {
    struct btrfs_chunk_list * result = chunk_list;
    struct btrfs_header * header = btrfs_chunk_list_resolve(chunk_list, data, chunk_root);
    struct btrfs_key_pointer * key_ptr;
    struct btrfs_item * item;
    struct btrfs_key key_buf;
    u32 nritems, i;
    u8 level;

    if (!header) {
        return chunk_list;
    }

    nritems = btrfs_header_nritems(header);
    level = btrfs_header_level(header);

    if (level > 0) {
        key_ptr = (void *) (header + 1);

        for (i = 0; i < nritems; ++i, ++key_ptr) {
            result = btrfs_read_chunk_tree(result, data, btrfs_key_pointer_blocknr(key_ptr));

            if (!result) {
                return chunk_list;
            }
        }
    } else {
        item = (void *) (header + 1);

        for (i = 0; i < nritems; ++i, ++item) {
            btrfs_disk_key_to_cpu(&key_buf, &item->key);

            if (key_buf.type != BTRFS_CHUNK_ITEM_KEY) {
                continue;
            }

            result = btrfs_chunk_list_new(
                    key_buf,
                    (void *) ((u8 *) (header + 1) + btrfs_item_offset(item)),
                    result
            );

            if (!result) {
                return chunk_list;
            }
        }
    }

    return result;
}

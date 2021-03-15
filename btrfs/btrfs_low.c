#include "btrfs_low.h"

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "btrfs_find_in_btree.h"
#include "btrfs_traverse_btree.h"
#include "struct/btrfs_key_pointer.h"
#include "struct/btrfs_root_item.h"
#include "struct/btrfs_disk_key.h"
#include "struct/btrfs_dir_item.h"
#include "struct/btrfs_header.h"
#include "struct/btrfs_chunk.h"
#include "struct/btrfs_item.h"
#include "lib/crc32c.h"


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

static inline u64 btrfs_name_hash(const char * name, int len) {
    return crc32c((u32) ~1, name, len);
}

static inline const char * find_filename_end(const char * path) {
    const char * end = strchr(path, '/');

    return end ? end : path + strlen(path);
}

int btrfs_low_locate_file(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 fs_tree,
        const char * path,
        struct btrfs_low_file_id * result
) {
    struct btrfs_dir_item * dir_item;
    struct btrfs_key key, key_buf;
    const char * end;

    key.objectid = BTRFS_FIRST_FREE_OBJECTID;
    key.type = BTRFS_DIR_ITEM_KEY;

    do {
        end = find_filename_end(path);
        key.offset = btrfs_name_hash(path, end - path);

        dir_item = btrfs_find_in_btree(chunk_list, data, fs_tree, key, NULL);
        if (!dir_item) {
            return -ENOENT;
        }

        btrfs_disk_key_to_cpu(&key_buf, &dir_item->location);
        key.objectid = key_buf.objectid;

        if (key_buf.type != BTRFS_INODE_ITEM_KEY) {
            /* TODO handle */
            exit(111);
        }

        path = end + 1;
    } while (*end);

    result->fs_tree = fs_tree;
    result->dir_item = key.objectid;

    return 0;
}

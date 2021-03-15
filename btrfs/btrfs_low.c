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


struct __btrfs_low_find_tree_root_acc {
    u64 objectid;
    u64 result;
};

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

static enum btrfs_traverse_btree_handler_result __btrfs_low_find_tree_root(
        struct __btrfs_low_find_tree_root_acc * acc,
        struct btrfs_key item_key,
        void * item_data
) {
    if (item_key.type != BTRFS_ROOT_ITEM_KEY) {
        btrfs_traverse_btree_continue;
    }

    if (item_key.objectid != acc->objectid) {
        btrfs_traverse_btree_continue;
    }

    acc->result = btrfs_root_item_bytenr((struct btrfs_root_item *) item_data);
    btrfs_traverse_btree_break;
}

static u64 btrfs_low_find_tree_root(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 root,
        u64 objectid
) {
    struct __btrfs_low_find_tree_root_acc acc = { .objectid = objectid, .result = 0 };

    btrfs_debug_printf("Find tree #%llu root:\n", objectid);
    btrfs_traverse_btree(chunk_list, data, root, &acc, __btrfs_low_find_tree_root);

    return acc.result;
}

u64 btrfs_low_find_root_fs_tree_root(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 root
) {
    return btrfs_low_find_tree_root(chunk_list, data, root, BTRFS_FS_TREE_OBJECTID);
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
        u64 root_tree,
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

        if (key_buf.type == BTRFS_INODE_ITEM_KEY) {
            key.objectid = key_buf.objectid;
        } else if (key_buf.type == BTRFS_ROOT_ITEM_KEY) {
            fs_tree = btrfs_low_find_tree_root(chunk_list, data, root_tree, key_buf.objectid);
            key.objectid = BTRFS_FIRST_FREE_OBJECTID;
        } else {
            return -EIO;
        }

        path = end + 1;
    } while (*end);

    result->fs_tree = fs_tree;
    result->dir_item = key.objectid;
    return 0;
}

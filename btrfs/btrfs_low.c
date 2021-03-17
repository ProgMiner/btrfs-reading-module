#include "btrfs_low.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "btrfs_find_in_btree.h"
#include "btrfs_traverse_btree.h"
#include "struct/btrfs_file_extent_item.h"
#include "struct/btrfs_key_pointer.h"
#include "struct/btrfs_inode_item.h"
#include "struct/btrfs_root_item.h"
#include "struct/btrfs_disk_key.h"
#include "struct/btrfs_dir_item.h"
#include "struct/btrfs_timespec.h"
#include "struct/btrfs_header.h"
#include "struct/btrfs_chunk.h"
#include "struct/btrfs_item.h"
#include "lib/crc32c.h"


struct __btrfs_low_locate_file_acc {
    u64 objectid;
    u64 hash;
    const char * filename;
    size_t filename_length;
    struct btrfs_dir_item * result;
};

struct __btrfs_low_list_files_acc {
    u64 objectid;
    char ** result;
    size_t result_length;
    size_t result_capacity;
    int ret;
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

    btrfs_debug_indent();
    btrfs_debug_printf("Reading chunk tree:\n");
    btrfs_traverse_btree(chunk_list, data, chunk_root, &result, __btrfs_low_read_chunk_tree_handler);

    return result;
}

static u64 btrfs_low_find_tree_root(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 root,
        u64 objectid
) {
    struct btrfs_root_item * root_item;
    struct btrfs_key key = {
        .objectid = objectid,
        .type = BTRFS_ROOT_ITEM_KEY,
        .offset = -1ULL
    };

    btrfs_debug_indent();
    btrfs_debug_printf("Find tree #%llu root:\n", objectid);
    root_item = btrfs_find_in_btree(chunk_list, data, root, key, NULL, false);

    return root_item ? root_item->bytenr : 0;
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

static enum btrfs_traverse_btree_handler_result __btrfs_low_locate_file_handler(
        struct __btrfs_low_locate_file_acc * acc,
        struct btrfs_key item_key,
        void * item_data
) {
    struct btrfs_dir_item * dir_item;

    if (item_key.type != BTRFS_DIR_ITEM_KEY) {
        btrfs_traverse_btree_continue;
    }

    if (item_key.objectid != acc->objectid) {
        btrfs_traverse_btree_continue;
    }

    if (item_key.offset != acc->hash) {
        btrfs_traverse_btree_continue;
    }

    dir_item = item_data;
    if (btrfs_dir_item_name_len(dir_item) != acc->filename_length) {
        btrfs_traverse_btree_continue;
    }

    if (strncmp((const char *) (dir_item + 1), acc->filename, acc->filename_length) != 0) {
        btrfs_traverse_btree_continue;
    }

    acc->result = dir_item;
    btrfs_traverse_btree_break;
}

int btrfs_low_locate_file(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        u64 root_tree,
        u64 fs_tree,
        const char * path,
        struct btrfs_low_file_id * result
) {
    struct __btrfs_low_locate_file_acc acc;
    struct btrfs_key key;
    const char * end;

    acc.objectid = BTRFS_FIRST_FREE_OBJECTID;
    if (!*path) {
        goto end;
    }

    do {
        end = find_filename_end(path);
        acc.hash = btrfs_name_hash(path, end - path);

        key.objectid = acc.objectid;
        key.type = BTRFS_DIR_ITEM_KEY;
        key.offset = acc.hash;

        /* check is any DIR_ITEM with specified hash exists */
        if (!btrfs_find_in_btree_exact(chunk_list, data, fs_tree, key, NULL)) {
            return -ENOENT;
        }

        acc.filename = path;
        acc.filename_length = end - path;
        acc.result = NULL;

        btrfs_traverse_btree(chunk_list, data, fs_tree, &acc, __btrfs_low_locate_file_handler);
        if (!acc.result) {
            return -ENOENT;
        }

        btrfs_disk_key_to_cpu(&key, &acc.result->location);
        if (key.type == BTRFS_INODE_ITEM_KEY) {
            acc.objectid = key.objectid;
        } else if (key.type == BTRFS_ROOT_ITEM_KEY) {
            fs_tree = btrfs_low_find_tree_root(chunk_list, data, root_tree, key.objectid);
            acc.objectid = BTRFS_FIRST_FREE_OBJECTID;
        } else {
            /* there could not be anything else */
            return -EIO;
        }

        path = end + 1;
    } while (*end);

end:
    result->fs_tree = fs_tree;
    result->dir_item = acc.objectid;
    return 0;
}

int btrfs_low_stat(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        struct btrfs_low_file_id file_id,
        struct stat * stat
) {
    struct btrfs_inode_item * inode;
    struct btrfs_key key = {
        .objectid = file_id.dir_item,
        .type = BTRFS_INODE_ITEM_KEY,
        .offset = 0
    };

    inode = btrfs_find_in_btree(chunk_list, data, file_id.fs_tree, key, NULL, true);
    if (!inode) {
        return -ENOENT;
    }

    memset(stat, 0, sizeof(struct stat));

    stat->st_ino = file_id.dir_item;
    stat->st_mode = btrfs_inode_item_mode(inode);
    stat->st_nlink = btrfs_inode_item_nlink(inode);
    stat->st_uid = btrfs_inode_item_uid(inode);
    stat->st_gid = btrfs_inode_item_gid(inode);
    stat->st_rdev = btrfs_inode_item_rdev(inode);
    stat->st_size = btrfs_inode_item_size(inode);
    stat->st_blocks = btrfs_inode_item_nbytes(inode) / 512;
    stat->st_atime = btrfs_timespec_sec(&inode->atime);
    stat->st_mtime = btrfs_timespec_sec(&inode->mtime);
    stat->st_ctime = btrfs_timespec_sec(&inode->ctime);
    stat->st_atimensec = btrfs_timespec_nsec(&inode->atime);
    stat->st_mtimensec = btrfs_timespec_nsec(&inode->mtime);
    stat->st_ctimensec = btrfs_timespec_nsec(&inode->ctime);

    /* because of .. */
    if (S_ISDIR(stat->st_mode)) {
        ++stat->st_nlink;
    }

    return 0;
}

static enum btrfs_traverse_btree_handler_result __btrfs_low_list_files_handler(
        struct __btrfs_low_list_files_acc * acc,
        struct btrfs_key item_key,
        void * item_data
) {
    struct btrfs_dir_item * dir_item;
    char ** new_result;
    u16 name_len;

    if (item_key.type != BTRFS_DIR_ITEM_KEY) {
        btrfs_traverse_btree_continue;
    }

    if (item_key.objectid != acc->objectid) {
        btrfs_traverse_btree_continue;
    }

    dir_item = item_data;

    /* bug if capacity < length */
    assert(acc->result_capacity >= acc->result_length);

    /* if array is full */
    if (acc->result_capacity == acc->result_length) {
        /* increase capacity by powers of 2 */

        acc->result_capacity <<= 1;
        new_result = realloc(acc->result, acc->result_capacity * sizeof(char *));

        if (!new_result) {
            acc->ret = -ENOMEM;
            btrfs_traverse_btree_break;
        }

        acc->result = new_result;
    }

    name_len = btrfs_dir_item_name_len(dir_item);
    acc->result[acc->result_length] = malloc((name_len + 1) * sizeof(char));
    if (!acc->result[acc->result_length]) {
        acc->ret = -ENOMEM;
        btrfs_traverse_btree_break;
    }

    strncpy(acc->result[acc->result_length], (const char *) (dir_item + 1), name_len + 1);
    acc->result[acc->result_length][name_len] = '\0';
    ++acc->result_length;

    btrfs_traverse_btree_continue;
}

int btrfs_low_list_files(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        struct btrfs_low_file_id dir_id,
        size_t * length,
        char *** files
) {
    char ** result = malloc(4 * sizeof(char *));
    struct __btrfs_low_list_files_acc acc = {
        .objectid = dir_id.dir_item,
        .result = result,
        .result_length = 2,
        .result_capacity = 4,
        .ret = 0
    };

    int ret = 0;
    size_t i;

    if (!result) {
        return -ENOMEM;
    }

    result[0] = malloc(2 * sizeof(char));
    result[1] = malloc(3 * sizeof(char));

    if (!result[0] || !result[1]) {
        ret = -ENOMEM;
        goto free_result;
    }

    strncpy(result[0],  ".", 2 * sizeof(char));
    strncpy(result[1], "..", 3 * sizeof(char));

    btrfs_traverse_btree(chunk_list, data, dir_id.fs_tree, &acc, __btrfs_low_list_files_handler);

    /* bug if length < capacity */
    assert(acc.result_length <= acc.result_capacity);

    ret = acc.ret;
    if (ret) {
        goto free_result;
    }

    if (acc.result_length < acc.result_capacity) {
        /* shrink array size to length */

        result = realloc(acc.result, acc.result_length * sizeof(char *));
        assert(result != NULL);
    }

    *files = acc.result;
    *length = acc.result_length;
    goto end;

free_result:
    for (i = 0; i < acc.result_length; ++i) {
        free(acc.result[i]);
    }

    free(acc.result);

end:
    return ret;
}

/* read length bytes from extent data starting from offset,
 * return count of read bytes or error
 */
static int __btrfs_low_read(
        struct btrfs_file_extent_item * extent_data,
        char * buf,
        size_t length,
        size_t offset
) {
    const char * data;
    size_t data_length;

    switch (btrfs_file_extent_item_type(extent_data)) {
    case BTRFS_FILE_EXTENT_INLINE:
        data = (void *) &extent_data->disk_bytenr;
        data_length = btrfs_file_extent_item_ram_bytes(extent_data);
        break;

    default:
        /* TODO implement */
        return -ENOENT;
    }

    if (offset < data_length) {
        if (offset + length > data_length) {
            length = data_length - offset;
        }

        memcpy(buf, data + offset, length);
    } else {
        length = 0;
    }

    return length;
}

int btrfs_low_read(
        struct btrfs_chunk_list * chunk_list,
        void * data,
        struct btrfs_low_file_id file_id,
        char * buf,
        size_t length,
        off_t offset
) {
    int ret = 0;
    size_t bytes_read = 0;
    struct btrfs_file_extent_item * extent_data;
    struct btrfs_key key = { .objectid = file_id.dir_item, .type = BTRFS_EXTENT_DATA_KEY };

    while (bytes_read < length) {
        key.offset = offset;
        extent_data = btrfs_find_in_btree(chunk_list, data, file_id.fs_tree, key, &key, false);

        if (extent_data == NULL
            || key.type != BTRFS_EXTENT_DATA_KEY
            || key.objectid != file_id.dir_item) {
            ret = -ENOENT;
            goto end;
        }

        /* bug if we found key with offset > query offset */
        assert(key.offset <= offset);

        ret = __btrfs_low_read(extent_data, buf, length - bytes_read, offset - key.offset);
        if (ret < 0) {
            goto end;
        }

        /* if we cannot read any bytes, file ended */
        if (ret == 0) {
            break;
        }

        /* bug if we read more than queried bytes */
        assert(ret <= length - bytes_read);

        offset += ret;
        bytes_read += ret;
        buf += ret;
        ret = 0;
    }

    ret = bytes_read;

end:
    return ret;
}

#pragma once

#include <sys/stat.h>
#include <sys/types.h>

#include "struct/btrfs_super_block.h"
#include "btrfs_chunk_list.h"
#include "types.h"


/* unique identifier for any file in btrfs */
struct btrfs_low_file_id {

    /* bytenr of fs_tree root */
    u64 fs_tree;

    /* objectid of inode_item */
    u64 dir_item;
};

const struct btrfs_super_block * btrfs_low_find_superblock(const void * data, size_t length);

struct btrfs_chunk_list * btrfs_low_read_sys_array(const struct btrfs_super_block * sb);

struct btrfs_chunk_list * btrfs_low_read_chunk_tree(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 chunk_root
);

u64 btrfs_low_find_root_fs_tree_root(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 root
);

int btrfs_low_locate_file(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 root_tree,
        u64 fs_tree,
        const char * path,
        struct btrfs_low_file_id * result
);

int btrfs_low_stat(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        struct btrfs_low_file_id file_id,
        struct stat * stat
);

int btrfs_low_list_files(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        struct btrfs_low_file_id dir_id,
        size_t * length,
        char *** files
);

int btrfs_low_read(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        struct btrfs_low_file_id file_id,
        char * content,
        size_t length,
        off_t offset
);

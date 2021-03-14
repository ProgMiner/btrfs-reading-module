#include "btrfs.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "btrfs_low.h"
#include "struct/btrfs_header.h"
#include "btrfs_debug.h"


struct btrfs * btrfs_openfs(void * data) {
    struct btrfs * btrfs = malloc(sizeof(btrfs));

    if (!btrfs) {
        return NULL;
    }

    btrfs->data = data;
    btrfs->sb = btrfs_low_find_superblock(data);

    if (!btrfs->sb) {
        goto error;
    }

    btrfs_debug_printf("Before reading sys array:\n");
    btrfs_chunk_list_print(btrfs->chunk_list);

    btrfs->chunk_list = btrfs_low_read_sys_array(btrfs->sb);

    btrfs_debug_printf("After reading sys array:\n");
    btrfs_chunk_list_print(btrfs->chunk_list);

    if (!btrfs->chunk_list) {
        goto error;
    }

    btrfs->chunk_list = btrfs_low_read_chunk_tree(
            btrfs->chunk_list,
            data,
            btrfs_super_block_chunk_root(btrfs->sb)
    );

    btrfs_debug_printf("After reading chunk tree:\n");
    btrfs_chunk_list_print(btrfs->chunk_list);

    btrfs->root_fs_tree_root = btrfs_low_find_root_fs_tree_root(
            btrfs->chunk_list,
            data,
            btrfs_super_block_root(btrfs->sb)
    );

    btrfs_debug_printf("%llu", btrfs->root_fs_tree_root);

    return btrfs;

error:
    free(btrfs);
    return NULL;
}

void btrfs_delete(struct btrfs * btrfs) {
    btrfs_chunk_list_delete(btrfs->chunk_list);
    free(btrfs);
}

int btrfs_stat(struct btrfs * btrfs, const char * filename, struct stat * stat) {
    if (stat != NULL) {
        memset(stat, 0, sizeof(struct stat));
    }

    if (strcmp(filename, "/") == 0) {
        if (stat != NULL) {
            stat->st_mode = S_IFDIR | 0755;
            stat->st_nlink = 2;
        }
    } else {
        if (stat != NULL) {
            stat->st_mode = S_IFREG | 0444;
            stat->st_nlink = 1;
            stat->st_size = strlen("test");
        }
    }

    return 0;
}

size_t btrfs_readdir(struct btrfs * btrfs, const char * filename, const char *** buf) {
    const char ** contents = malloc(3 * sizeof(char *));

    contents[0] = ".";
    contents[1] = "..";
    contents[2] = "hello";

    *buf = contents;
    return 3;
}

size_t btrfs_read(struct btrfs * btrfs, const char * filename, char ** buf) {
    char * data = strdup("test");

    *buf = data;
    return 5;
}

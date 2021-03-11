#include "btrfs.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "btrfs_low.h"


struct btrfs * btrfs_openfs(void * data) {
    struct btrfs * btrfs = malloc(sizeof(btrfs));

    if (!btrfs) {
        return NULL;
    }

    btrfs->data = data;
    btrfs->sb = btrfs_low_find_superblock(data);

    if (!btrfs->sb) {
        free(btrfs);
        return NULL;
    }

    btrfs->chunk_list = btrfs_read_sys_array(btrfs->sb);

    return btrfs;
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

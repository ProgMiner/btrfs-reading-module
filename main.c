#define _LARGEFILE64_SOURCE
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "btrfs.h"


static int btrfs_fd;
static void * btrfs_data = NULL;
static off64_t btrfs_data_length;
static struct btrfs * btrfs = NULL;

static int btrfs_fuse_getattr(const char * path, struct stat * stbuf) {
    return btrfs_stat(btrfs, path, stbuf);
}

static int btrfs_fuse_readdir(
        const char * path,
        void * buf,
        fuse_fill_dir_t filler,
        off_t offset,
        struct fuse_file_info * fi
) {
    const char ** contents;
    size_t count = btrfs_readdir(btrfs, path, &contents);
    size_t i;

    (void) offset;
    (void) fi;

    if (contents == NULL) {
        return -ENOENT;
    }

    for (i = 0; i < count; ++i) {
        filler(buf, contents[i], NULL, 0);
    }

    free(contents);
    return 0;
}

static int btrfs_fuse_open(const char * path, struct fuse_file_info * fi) {
    (void) fi;

    return btrfs_stat(btrfs, path, NULL);
}

static int btrfs_fuse_read(
        const char * path,
        char * buf,
        size_t size,
        off_t offset,
        struct fuse_file_info * fi
) {
    char * data;
    size_t len = btrfs_read(btrfs, path, &data);
    (void) fi;

    if (data == NULL) {
        return -ENOENT;
    }

    if (offset < len) {
        if (offset + size > len) {
            size = len - offset;
        }

        memcpy(buf, data + offset, size);
    } else {
        size = 0;
    }

    free(data);
    return size;
}

static void btrfs_fuse_destroy(void * private_data) {
    (void) private_data;

    btrfs_delete(btrfs);

    munmap(btrfs_data, btrfs_data_length);
    close(btrfs_fd);
}

static struct fuse_operations btrfs_fuse_oper = {
    .getattr    = btrfs_fuse_getattr,
    .readdir    = btrfs_fuse_readdir,
    .open       = btrfs_fuse_open,
    .read       = btrfs_fuse_read,
    .destroy    = btrfs_fuse_destroy,
};

int main(int argc, char * argv[]) {
    btrfs_fd = open("testfs", 0, O_RDONLY);
    btrfs_data_length = lseek64(btrfs_fd, 0, SEEK_END);

    if (btrfs_data_length < 0) {
        return errno;
    }

    btrfs_data = mmap(NULL, btrfs_data_length, PROT_READ, MAP_PRIVATE, btrfs_fd, 0);
    btrfs = btrfs_openfs(btrfs_data);

    /// TODO remove

    (void) btrfs_fuse_oper;
    btrfs_fuse_destroy(NULL);
    return 0;

    /// TODO /remove

    // return fuse_main(argc, argv, &btrfs_fuse_oper, NULL);
}

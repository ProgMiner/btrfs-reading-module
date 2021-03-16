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
#include <assert.h>

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
    char ** contents;
    size_t count, i;
    int ret = 0;

    (void) offset;
    (void) fi;

    ret = btrfs_readdir(btrfs, path, &count, &contents);
    if (ret) {
        goto end;
    }

    for (i = 0; i < count; ++i) {
        filler(buf, contents[i], NULL, 0);
        free(contents[i]);
    }

    free(contents);

end:
    return ret;
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
    int ret = btrfs_read(btrfs, path, buf, size, offset);
    size_t read_len;

    (void) fi;

    if (ret < 0) {
        read_len = 0;
    } else {
        read_len = ret;
    }

    assert(read_len <= size);

    if (read_len < size) {
        memset(buf + read_len, 0, size - read_len);
    }

    return ret;
}

static int btrfs_fuse_readlink(
        const char * path,
        char * buf,
        size_t size
) {
    return btrfs_readlink(btrfs, path, buf, size);
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
    .readlink   = btrfs_fuse_readlink,
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

    return fuse_main(argc, argv, &btrfs_fuse_oper, NULL);
}

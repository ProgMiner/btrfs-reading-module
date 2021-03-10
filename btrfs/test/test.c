#define _LARGEFILE64_SOURCE

#include "../include/btrfs.h"

#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


static int btrfs_fd;
static void * btrfs_data = NULL;
static off64_t btrfs_data_length;
static struct btrfs * btrfs = NULL;

int main(int argc, const char ** argv) {
    int ret = 0;

    if (argc < 2) {
        fprintf(stderr, "Not enough args\n");
        return -1;
    }

    btrfs_fd = open(argv[1], 0, O_RDONLY);
    if (btrfs_fd == -1) {
        fprintf(stderr, "Couldn't open file\n");
        return errno;
    }

    btrfs_data_length = lseek64(btrfs_fd, 0, SEEK_END);
    if (btrfs_data_length < 0) {
        fprintf(stderr, "Couldn't get size of file\n");
        ret = errno;
        goto close;
    }

    btrfs_data = mmap(NULL, btrfs_data_length, PROT_READ, MAP_PRIVATE, btrfs_fd, 0);
    if (btrfs_data == (void *) -1) {
        fprintf(stderr, "Couldn't map fs to memory\n");
        ret = errno;
        goto close;
    }

    btrfs = btrfs_openfs(btrfs_data);
    if (!btrfs) {
        fprintf(stderr, "Couldn't init btrfs struct\n");
        ret = -1;
        goto free_btrfs;
    }

    // TODO

free_btrfs:
    btrfs_delete(btrfs);

close:
    munmap(btrfs_data, btrfs_data_length);
    close(btrfs_fd);
    return ret;
}

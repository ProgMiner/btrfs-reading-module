#define _LARGEFILE64_SOURCE

#include "../include/btrfs.h"

#include <stdio.h>
#include <stdlib.h>
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
    size_t length, i;
    char ** files;
    char * file;
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

    btrfs = btrfs_openfs(btrfs_data, btrfs_data_length);
    if (!btrfs) {
        fprintf(stderr, "Couldn't init btrfs struct\n");
        ret = -1;
        goto close;
    }

    ret |= btrfs_stat(btrfs, "/", NULL);
    ret |= btrfs_stat(btrfs, "/f1", NULL);
    ret |= btrfs_stat(btrfs, "/l1", NULL);
    ret |= btrfs_stat(btrfs, "/l2", NULL);
    ret |= btrfs_stat(btrfs, "/d1", NULL);
    ret |= btrfs_stat(btrfs, "/d1/f1", NULL);
    ret |= btrfs_stat(btrfs, "/d2/l1", NULL);
    ret |= btrfs_stat(btrfs, "/ext2_saved", NULL);
    ret |= btrfs_stat(btrfs, "/ext2_saved/image", NULL);

    if (ret) {
        ret = -1;
        goto free_btrfs;
    }

    printf("All test files found!\n");

    ret = btrfs_readdir(btrfs, "/", &length, &files);
    if (ret) {
        goto free_btrfs;
    }

    printf("Root directory contents:\n");
    for (i = 0; i < length; ++i) {
        printf("%zu. %s\n", i + 1, files[i]);
        free(files[i]);
    }

    free(files);

    ret = btrfs_readdir(btrfs, "/ext2_saved", &length, &files);
    if (ret) {
        goto free_btrfs;
    }

    printf("Subvolume root directory contents:\n");
    for (i = 0; i < length; ++i) {
        printf("%zu. %s\n", i + 1, files[i]);
        free(files[i]);
    }

    free(files);

    file = malloc(10 * sizeof(char));
    ret = btrfs_read(btrfs, "/f1", file, 10, 0);
    if (ret < 0) {
        goto free_btrfs;
    }

    file[ret] = '\0';
    printf("Read %d bytes from /f1: \"%s\"\n", ret, file);

    ret = btrfs_read(btrfs, "/f1", file, 4, 0);
    if (ret < 0) {
        goto free_btrfs;
    }

    file[ret] = '\0';
    printf("Read %d bytes from /f1: \"%s\"\n", ret, file);

    ret = btrfs_read(btrfs, "/f1", file, 2, 0);
    if (ret < 0) {
        goto free_btrfs;
    }

    file[ret] = '\0';
    printf("Read %d bytes from /f1: \"%s\"\n", ret, file);


    ret = btrfs_read(btrfs, "/f1", file, 2, 2);
    if (ret < 0) {
        goto free_btrfs;
    }

    file[ret] = '\0';
    printf("Read %d bytes from /f1: \"%s\"\n", ret, file);
    ret = 0;

free_btrfs:
    btrfs_delete(btrfs);

close:
    munmap(btrfs_data, btrfs_data_length);
    close(btrfs_fd);
    return ret;
}

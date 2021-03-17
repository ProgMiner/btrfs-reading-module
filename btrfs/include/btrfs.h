#pragma once

#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>


struct btrfs;

/* read btrfs from data */
struct btrfs * btrfs_openfs(void * data, size_t length);

/* delete btrfs structure */
void btrfs_delete(struct btrfs * btrfs);

/* stat file on btrfs or check is it exists (stat = NULL) */
int btrfs_stat(struct btrfs * btrfs, const char * filename, struct stat * stat);

/* get list of files in directory */
int btrfs_readdir(
        struct btrfs * btrfs,
        const char * filename,
        size_t * length,
        char *** contents
);

/* read piece of file contents, returns count of bytes read or error */
int btrfs_read(
        struct btrfs * btrfs,
        const char * filename,
        char * data,
        size_t length,
        off_t offset
);

/* read symlink */
int btrfs_readlink(
        struct btrfs * btrfs,
        const char * filename,
        char * data,
        size_t length
);

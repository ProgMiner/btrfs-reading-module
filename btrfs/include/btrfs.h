#pragma once

#include <stddef.h>
#include <sys/stat.h>


struct btrfs;

struct btrfs * btrfs_openfs(void * data);
void btrfs_delete(struct btrfs * btrfs);

int btrfs_stat(struct btrfs * btrfs, const char * filename, struct stat * stat);

int btrfs_readdir(
        struct btrfs * btrfs,
        const char * filename,
        size_t * length,
        const char *** contents
);

int btrfs_read(struct btrfs * btrfs, const char * filename, size_t * length, char ** data);

#pragma once

#include <stddef.h>
#include <sys/stat.h>


struct btrfs;

struct btrfs * btrfs_openfs(void * data);
void btrfs_delete(struct btrfs * btrfs);

int btrfs_stat(struct btrfs * btrfs, const char * filename, struct stat * stat);

size_t btrfs_readdir(struct btrfs * btrfs, const char * filename, const char *** contents);
size_t btrfs_read(struct btrfs * btrfs, const char * filename, char ** data);

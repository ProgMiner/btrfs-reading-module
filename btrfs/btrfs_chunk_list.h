#pragma once

#include "struct/btrfs_key.h"
#include "struct/btrfs_chunk.h"


struct btrfs_chunk_list;

struct btrfs_chunk_list * btrfs_chunk_list_new(
        struct btrfs_key key,
        struct btrfs_chunk * chunk,
        struct btrfs_chunk_list * next
);

void btrfs_chunk_list_delete(struct btrfs_chunk_list * list);

/* TODO several devs */
void * btrfs_chunk_list_resolve(struct btrfs_chunk_list * list, void * data, u64 logical);

void btrfs_chunk_list_print(struct btrfs_chunk_list * list);

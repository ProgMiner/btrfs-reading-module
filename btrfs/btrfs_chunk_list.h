#pragma once

#include "struct/btrfs_key.h"
#include "struct/btrfs_chunk.h"


struct btrfs_chunk_list;

struct btrfs_chunk_list * btrfs_chunk_list_new(
        struct btrfs_key key,
        const struct btrfs_chunk * chunk,
        struct btrfs_chunk_list * next
);

void btrfs_chunk_list_delete(struct btrfs_chunk_list * list);

/* TODO several devs */
const void * btrfs_chunk_list_resolve(
        const struct btrfs_chunk_list * list,
        const void * data,
        u64 logical
);

#ifdef BTRFS_DEBUG
void btrfs_chunk_list_print(const struct btrfs_chunk_list * list);
#else
#define btrfs_chunk_list_print(__list)
#endif

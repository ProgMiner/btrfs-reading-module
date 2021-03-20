#pragma once

#include <stdbool.h>

#include "types.h"
#include "btrfs_chunk_list.h"
#include "struct/btrfs_key.h"


#define btrfs_find_in_btree_exact(__chunk_list, __data, __btree_root, __key, __result) \
    btrfs_find_in_btree(__chunk_list, __data, __btree_root, __key, __result, true)


/* b-tree search
 *
 * if item with specified key is present in tree, returns item and result is set to key,
 * otherwise returns NULL and result is not defined
 *
 * if exact is true finds not exactly key but item with maximal key
 * that obejectid and type is equals to query
 *
 * result can be NULL if you don't need to get key
 */
const void * btrfs_find_in_btree(
        const struct btrfs_chunk_list * chunk_list,
        const void * data,
        u64 btree_root,
        struct btrfs_key key,
        struct btrfs_key * result,
        bool exact
);

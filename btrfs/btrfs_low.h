#pragma once

#include "struct/btrfs_super_block.h"
#include "struct/btrfs_disk_key.h"
#include "struct/btrfs_chunk.h"
#include "btrfs_chunk_list.h"


struct btrfs_super_block * btrfs_low_find_superblock(void * data);

struct btrfs_chunk_list * btrfs_read_sys_array(struct btrfs_super_block * sb);

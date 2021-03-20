#include "btrfs.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "btrfs_low.h"
#include "struct/btrfs_header.h"
#include "btrfs_debug.h"


struct btrfs * btrfs_openfs(const void * data, size_t length) {
    struct btrfs * btrfs = malloc(sizeof(struct btrfs));
    const struct btrfs_super_block * sb;

    btrfs_debug_start_section("btrfs_openfs");

    if (!btrfs) {
        btrfs_debug_indent();
        btrfs_debug_printf("Couldn't allocate memory\n");
        goto bad_end;
    }

    btrfs->data = data;
    sb = btrfs_low_find_superblock(data, length);

    if (!sb) {
        btrfs_debug_indent();
        btrfs_debug_printf("Couldn't find superblock\n");
        goto free_btrfs;
    }

    btrfs_debug_indent();
    btrfs_debug_printf("Before reading sys array:\n");
    btrfs_chunk_list_print(btrfs->chunk_list);

    btrfs->chunk_list = btrfs_low_read_sys_array(sb);

    btrfs_debug_indent();
    btrfs_debug_printf("After reading sys array:\n");
    btrfs_chunk_list_print(btrfs->chunk_list);

    if (!btrfs->chunk_list) {
        btrfs_debug_indent();
        btrfs_debug_printf("Couldn't read sys array\n");
        goto free_btrfs;
    }

    btrfs->chunk_list = btrfs_low_read_chunk_tree(
            btrfs->chunk_list,
            data,
            btrfs_super_block_chunk_root(sb)
    );

    btrfs_debug_indent();
    btrfs_debug_printf("After reading chunk tree:\n");
    btrfs_chunk_list_print(btrfs->chunk_list);

    btrfs->root_tree = btrfs_super_block_root(sb);
    btrfs->root_fs_tree = btrfs_low_find_root_fs_tree_root(
            btrfs->chunk_list,
            data,
            btrfs->root_tree
    );

    if (!btrfs->root_fs_tree) {
        btrfs_debug_indent();
        btrfs_debug_printf("Couldn't find root FS_TREE root\n");
        goto free_chunks;
    }

    btrfs_debug_indent();
    btrfs_debug_printf("Found root FS_TREE root at bytenr %llu\n", btrfs->root_fs_tree);
    goto end;

free_chunks:
    btrfs_chunk_list_delete(btrfs->chunk_list);

free_btrfs:
    free(btrfs);

bad_end:
    btrfs = NULL;

end:
    btrfs_debug_end_section("btrfs_openfs");
    return btrfs;
}

void btrfs_delete(struct btrfs * btrfs) {
    btrfs_chunk_list_delete(btrfs->chunk_list);
    free(btrfs);
}

static int btrfs_get_file_id(
        const struct btrfs * btrfs,
        const char * filename,
        struct btrfs_low_file_id * file_id
) {
    int ret = 0;

    btrfs_debug_start_section("btrfs_get_file_id");

    if (filename[0] != '/') {
        btrfs_debug_indent();
        btrfs_debug_printf("Filename isn't starting with /\n");
        ret = -EINVAL;
        goto end;
    }

    filename += 1;
    if (btrfs_low_locate_file(
            btrfs->chunk_list,
            btrfs->data,
            btrfs->root_tree,
            btrfs->root_fs_tree,
            filename,
            file_id
    )) {
        btrfs_debug_indent();
        btrfs_debug_printf("Couldn't find file /%s\n", filename);
        ret = -ENOENT;
        goto end;
    }

    btrfs_debug_indent();
    btrfs_debug_printf("Found file /%s at: FS_TREE %llu, OBJECTID %llu\n",
            filename, file_id->fs_tree, file_id->dir_item);

end:
    btrfs_debug_end_section("btrfs_get_file_id");
    return ret;
}

int btrfs_stat(const struct btrfs * btrfs, const char * filename, struct stat * stat) {
    struct btrfs_low_file_id file_id;
    int ret = 0;

    ret = btrfs_get_file_id(btrfs, filename, &file_id);
    if (ret) {
        goto end;
    }

    if (stat) {
        ret = btrfs_low_stat(btrfs->chunk_list, btrfs->data, file_id, stat);
    }

end:
    return ret;
}

int btrfs_readdir(
        const struct btrfs * btrfs,
        const char * filename,
        size_t * length,
        char *** contents
) {
    struct btrfs_low_file_id file_id;
    int ret = 0;

    ret = btrfs_get_file_id(btrfs, filename, &file_id);
    if (ret) {
        goto end;
    }

    ret = btrfs_low_list_files(btrfs->chunk_list, btrfs->data, file_id, length, contents);

end:
    return ret;
}

int btrfs_read(
        const struct btrfs * btrfs,
        const char * filename,
        char * data,
        size_t length,
        off_t offset
) {
    struct btrfs_low_file_id file_id;
    int ret = 0;

    ret = btrfs_get_file_id(btrfs, filename, &file_id);
    if (ret) {
        goto end;
    }

    ret = btrfs_low_read(
            btrfs->chunk_list,
            btrfs->data,
            file_id,
            data,
            length,
            offset
    );

end:
    return ret;
}

int btrfs_readlink(
        const struct btrfs * btrfs,
        const char * filename,
        char * data,
        size_t length
) {
    int ret = btrfs_read(btrfs, filename, data, length, 0);

    if (ret < 0) {
        return ret;
    }

    if (ret < length) {
        data[ret] = '\0';
    }

    return 0;
}

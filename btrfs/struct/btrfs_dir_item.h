#pragma once

#include "../types.h"


#define BTRFS_FT_UNKNOWN  0
#define BTRFS_FT_REG_FILE 1
#define BTRFS_FT_DIR      2
#define BTRFS_FT_CHRDEV   3
#define BTRFS_FT_BLKDEV   4
#define BTRFS_FT_FIFO     5
#define BTRFS_FT_SOCK     6
#define BTRFS_FT_SYMLINK  7

#define BTRFS_FT_XATTR 8


struct btrfs_dir_item {

    /* key for the INODE_ITEM or ROOT_ITEM associated with this entry.
     * Unused and zeroed out when the entry describes an extended attribute
     */
    struct btrfs_disk_key location;

    /* transid of the transaction that created this entry */
    __le64 transid;

    /* length of the extended attrivute associated with this entry. 0 for standard directories */
    __le16 data_len;

    /* length of directory entry name */
    __le16 name_len;

    /* type
     *
     * Types:
     * These directory entry types use the same values as the d_type field in struct dirent
     * documented in the readdir(3) manual page.
     * 
     *   - BTRFS_FT_UNKNOWN = 0
     *     The target object's type is unknown. Indicates corruption if used.
     *   - BTRFS_FT_REG_FILE = 1
     *     The target object is an INODE_ITEM representing a regular file.
     *   - BTRFS_FT_DIR = 2
     *     The target object is an INODE_ITEM representing a directory or a ROOT_ITEM
     *     that will be presented as a directory.
     *   - BTRFS_FT_CHRDEV = 3
     *     The target object is an INODE_ITEM representing a character device node.
     *   - BTRFS_FT_BLKDEV = 4
     *     The target object is an INODE_ITEM representing a block device node.
     *   - BTRFS_FT_FIFO = 5
     *     The target object is an INODE_ITEM representing a FIFO device node.
     *   - BTRFS_FT_SOCK = 6
     *     The target object is an INODE_ITEM representing a socket device node.
     *   - BTRFS_FT_SYMLINK = 7
     *     The target object is an INODE_ITEM representing a symbolic link.
     * 
     * This value is used on-disk and internally but is not user-visible:
     *   - BTRFS_FT_XATTR = 8
     *     The entry is an XATTR_ITEM.
     */
    u8 type;
} __attribute__ ((__packed__));

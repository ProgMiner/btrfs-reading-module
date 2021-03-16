#pragma once

#include "../types.h"
#include "btrfs_timespec.h"


#define BTRFS_INODE_NODATASUM  0x001
#define BTRFS_INODE_NODATACOW  0x002
#define BTRFS_INODE_READONLY   0x004
#define BTRFS_INODE_NOCOMPRESS 0x008
#define BTRFS_INODE_PREALLOC   0x010
#define BTRFS_INODE_SYNC       0x020
#define BTRFS_INODE_IMMUTABLE  0x040
#define BTRFS_INODE_APPEND     0x080
#define BTRFS_INODE_NODUMP     0x100
#define BTRFS_INODE_NOATIME    0x200
#define BTRFS_INODE_DIRSYNC    0x400
#define BTRFS_INODE_COMPRESS   0x800

#define BTRFS_INODE_ROOT_ITEM_INIT (1ULL << 31)


struct btrfs_inode_item {

    /* nfs style generation number */
    __le64 generation;

    /* transid that last touched this inode */
    __le64 transid;

    /* stat.st_size; Size of the file in bytes */
    __le64 size;

    /* stat.st_blocks in byte units. Size allocated to this file, in bytes;
     * Sum of the offset fields of all EXTENT_DATA items for this inode. For directories: 0
     */
    __le64 nbytes;

    /* unused for normal inodes
     * Contains byte offset of block group when used as a free space inode
     */
    __le64 block_group;

    /* stat.st_nlink; Count of INODE_REF entries for the inode. When used outside of a file tree, 1 */
    __le32 nlink;

    /* stat.st_uid */
    __le32 uid;

    /* stat.st_gid */
    __le32 gid;

    /* stat.st_mode */
    __le32 mode;

    /* stat.st_rdev */
    __le64 rdev;

    /* inode flags
     *
     * Flags
     * NOTE: The unimplemented status of any of these flags was evaluated against Linux 4.11-rc2.
     * 
     *   - BTRFS_INODE_NODATASUM [0x1]
     *     Do not perform checksum operations on this inode.
     *   - BTRFS_INODE_NODATACOW [0x2]
     *     Do not perform CoW for data extents on this inode when the reference count is 1.
     *   - BTRFS_INODE_READONLY [0x4]
     *     Inode is read-only regardless of UNIX permissions or ownership.
     *     This bit is still checked and returns EACCES but there is no way to set it. That suggests that it has been superseded by BTRFS_INODE_IMMUTABLE.
     *   - BTRFS_INODE_NOCOMPRESS [0x8]
     *     Do not compress this inode.
     *     This flag may be changed by the kernel as compression ratios change. If the compression ratio for data associated with an inode becomes undesirable, this flag will be set. It may be cleared if the data changes and the compression ratio is favorable again.
     *   - BTRFS_INODE_PREALLOC [0x10]
     *     Inode contains preallocated extents. This instructs the kernel to attempt to avoid CoWing those extents.
     *   - BTRFS_INODE_SYNC [0x20]
     *     Operations on this inode will be performed synchronously.
     *     This flag is converted to a VFS-level inode flag but is not handled anywhere.
     *   - BTRFS_INODE_IMMUTABLE [0x40]
     *     Inode is read-only regardless of UNIX permissions or ownership. Attempts to modify this inode will result in EPERM being returned to the user.
     *   - BTRFS_INODE_APPEND [0x80]
     *     This inode is append-only.
     *   - BTRFS_INODE_NODUMP [0x100]
     *     This inode is not a candidate for dumping using the dump(8) program.
     *     This flag will be accepted on all kernels but is not implemented
     *   - BTRFS_INODE_NOATIME [0x200]
     *     Do not update atime] when this inode is accessed.
     *   - BTRFS_INODE_DIRSYNC [0x400]
     *     Operations on directory operations will be performed synchronously.
     *     This flag is converted to a VFS-level inode flag but is not handled anywhere.
     *   - BTRFS_INODE_COMPRESS [0x800]
     *     Compression is enabled on this inode.
     *
     *   - BTRFS_INODE_ROOT_ITEM_INIT [1 << 31]
     *     Flag to indicate that the flags and byte_limit flags have been properly initialized.
     */
    __le64 flags;

    /* sequence number used for NFS compatibility.
     * Initialized to 0 and incremented each time mtime value is changed
     *
     * modification sequence number for NFS
     */
    __le64 sequence;

    /* reserved for future use
     *
     * a little future expansion, for more than this we can
     * just grow the inode item and version it
     */
    __le64 reserved[4];

    /* stat.st_atime */
    struct btrfs_timespec atime;

    /* stat.st_ctime */
    struct btrfs_timespec ctime;

    /* stat.st_mtime */
    struct btrfs_timespec mtime;

    /* timestamp of inode creation */
    struct btrfs_timespec otime;
} __attribute__ ((__packed__));

static inline u32 btrfs_inode_item_mode(struct btrfs_inode_item * inode_item) {
    return le32_to_cpu(inode_item->mode);
}

static inline u32 btrfs_inode_item_nlink(struct btrfs_inode_item * inode_item) {
    return le32_to_cpu(inode_item->nlink);
}

static inline u32 btrfs_inode_item_uid(struct btrfs_inode_item * inode_item) {
    return le32_to_cpu(inode_item->uid);
}

static inline u32 btrfs_inode_item_gid(struct btrfs_inode_item * inode_item) {
    return le32_to_cpu(inode_item->gid);
}

static inline u64 btrfs_inode_item_rdev(struct btrfs_inode_item * inode_item) {
    return le64_to_cpu(inode_item->rdev);
}

static inline u64 btrfs_inode_item_size(struct btrfs_inode_item * inode_item) {
    return le64_to_cpu(inode_item->size);
}

static inline u64 btrfs_inode_item_nbytes(struct btrfs_inode_item * inode_item) {
    return le64_to_cpu(inode_item->nbytes);
}

#pragma once

#include "../types.h"


#define BTRFS_COMPRESS_NONE 0
#define BTRFS_COMPRESS_ZLIB 1
#define BTRFS_COMPRESS_LZO  2


#define BTRFS_FILE_EXTENT_INLINE   0
#define BTRFS_FILE_EXTENT_REG      1
#define BTRFS_FILE_EXTENT_PREALLOC 2


struct btrfs_file_extent_item {

	/* transaction id that created this extent */
	__le64 generation;

	/* upper limit on the size of the extent in ram
     *
	 * max number of bytes to hold this extent in ram
	 * when we split a compressed extent we can't know how big
	 * each of the resulting pieces will be.  So, this is
	 * an upper limit on the size of the extent in ram instead of
	 * an exact limit.
	 */
	__le64 ram_bytes;

	/* compression type. Can be one of:
     *   - BTRFS_COMPRESS_NONE (0)
     *   - BTRFS_COMPRESS_ZLIB (1)
     *   - BTRFS_COMPRESS_LZO (2)
     *
	 * 32 bits for the various ways we might encode the data,
	 * including compression and encryption. If any of these
	 * are set to something a given disk format doesn't understand
	 * it is treated like an incompat flag for reading and writing,
	 * but not for stat.
	 */
	u8 compression;

    /* encryption type, currently always set to 0 */
	u8 encryption;

    /* currently unused, allows for future expansion */
	__le16 other_encoding;

	/* are we inline data or a real extent?
     *
     * Type of extent. Can be:
     *   - BTRFS_FILE_EXTENT_INLINE (0)
     *   - BTRFS_FILE_EXTENT_REG (1)
     *   - BTRFS_FILE_EXTENT_PREALLOC (2)
     *
     * For inline extents the file contents immediately follow the type byte.
     * The length of the inline data block equals ram_bytes, alternatively it can be read
     * from the inode or computed as the size stored in the corresponding btrfs_item-structure
     * less the (partial) size of the btrfs_file_extent_item-structure.
     */
	u8 type;

    /* !!!
     * The following fields apply to
     * BTRFS_FILE_EXTENT_REG/BTRFS_FILE_EXTENT_PREALLOC extents only
     * !!!
     */

	/* logical address for the start of the extent data.
     * Note: This is the key.objectid for the corresponding EXTENT_ITEM
     *
	 * Disk space consumed by the data extent
	 * Data checksum is stored in csum tree, thus no bytenr/length takes
	 * csum into consideration.
	 *
	 * The inline extent data starts at this offset in the structure.
	 */
	__le64 disk_bytenr;

    /* number of on-disk bytes of the extent (compressed).
     * Note: This is the key.offset for the corresponding EXTENT_ITEM
     */
	__le64 disk_num_bytes;

	/* offset into the extent item. Number of bytes to skip at the beginning (uncompressed)
     *
	 * The logical offset in file blocks.
	 * this extent record is for.  This allows a file extent to point
	 * into the middle of an existing extent on disk, sharing it
	 * between two snapshots (useful if some bytes in the middle of the
	 * extent have changed
	 */
	__le64 offset;

	/* number of valid bytes within the file extent item (uncompressed).
     * This always reflects the size uncompressed and without encoding
     *
	 * The logical number of file blocks. This always reflects the size
	 * uncompressed and without encoding.
	 */
	__le64 num_bytes;
} __attribute__ ((__packed__));

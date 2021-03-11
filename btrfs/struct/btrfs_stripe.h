#pragma once

#include "../types.h"
#include "../btrfs_consts.h"


/*
 * This structure is used to define the backing device storage that compose a btrfs chunk.
 * It is associated with the CHUNK_ITEM item. This structure is never used outside of this item.
 */
struct btrfs_stripe {

    /* device ID that contains this stripe */
	__le64 devid;

    /* location of the start of the stripe, in bytes.
     * Size is determined by the stripe_len field in struct btrfs_chunk
     */
	__le64 offset;

    /* UUID of the device that contains this stripe.
     * Used to confirm that the correct device has been retrieved
     */
	u8 dev_uuid[BTRFS_UUID_SIZE];
} __attribute__ ((__packed__));

static inline u64 btrfs_stripe_offset(struct btrfs_stripe * stripe) {
    return le64_to_cpu(stripe->offset);
}

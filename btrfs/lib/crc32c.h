#pragma once

#include <stdint.h>
#include <stddef.h>


uint32_t calculate_crc32c(uint32_t crc32c, const unsigned char * buffer, unsigned int length);

static inline uint32_t crc32c(uint32_t crc, const char * buf, size_t len) {
    return calculate_crc32c(crc, (const unsigned char *) buf, len);
}

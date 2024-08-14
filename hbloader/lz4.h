#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

int lz4_decompress(const uint8_t *data, size_t data_length, uint8_t *dest,
                   size_t dest_length);

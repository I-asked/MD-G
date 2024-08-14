#include <assert.h>
#include "lz4.h"

int lz4_decompress(const uint8_t *data, size_t data_length, uint8_t *dest,
                   size_t dest_length) {
  size_t op = 0, ip = 0;
  for (;;) {
    const uint8_t token = data[ip++];
    int length = token >> 4;
    if (length == 15) {
      do {
        length += data[ip];
      } while (data[ip++] == 255);
    }
    while (--length >= 0 && op < dest_length) {
      dest[op++] = data[ip++];
    }
    if (ip >= data_length || op >= dest_length)
      break;

    int offset = data[ip] | (data[ip + 1] << 8);
    ip += 2;
    assert(offset != 0);

    size_t matchp = op - offset;
    int matchlength = (token & 15) + 4;
    if (matchlength == 19) {
      do {
        matchlength += data[ip];
      } while (data[ip++] == 255);
    }
    while (--matchlength >= 0 && op < dest_length) {
      dest[op++] = dest[matchp++];
    }
  }
  return op;
}

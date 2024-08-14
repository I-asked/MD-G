#include "util.h"

unsigned int djb2(const uint8_t *buf, int sz) {
  unsigned int hash = 5381;
  for (int i = 0; i < sz; ++i) {
    hash *= 33;
    hash += buf[i];
  }
  return hash;
}

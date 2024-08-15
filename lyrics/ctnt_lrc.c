#include "lrc.h"
#include "util.h"
#include "lz4.h"

#include "common.inl"

MY_API const DBuffer *read_lyrics(int track) {
  return NULL;
}

MY_API _Bool write_lyrics(int track, const CBuffer *comp) {
  return false;
}

#include "lrc.h"
#include "util.h"
#include <assert.h>
#include <sys/md.h>

typedef struct [[gnu::packed, gnu::aligned(1)]] TrackHeader {
  unsigned short magic : 16;
  unsigned int cksum : 24;
  uint8_t track : 8;
  unsigned short size : 15;
  _Bool enabled : 1;
} TrackHeader;
_Static_assert(sizeof(TrackHeader) == 8, "TrackHeader");


MY_API const DBuffer *read_lyrics(int track) {
  TrackHeader header;
  int j = 0;
  while (j < SEC_SZ) {
    atrac_read_metadata(SEC_META_TSTMP, j, (uint8_t *)&header, sizeof(header));
    if (header.size > SEC_SZ - j - sizeof(header) ||
        header.size > sizeof(g_cbuffer.data)) {
      return NULL;
    }
    if (header.magic != MY_MAGIC) {
      return NULL;
    }
    if (header.track == track && header.enabled) {
      g_cbuffer.size = header.size;
      atrac_read_metadata(SEC_META_TSTMP, j + sizeof(header), g_cbuffer.data,
                          g_cbuffer.size);
      if ((djb2(g_cbuffer.mem, sizeof(g_cbuffer.mem)) & 0xFFFFFF) !=
          header.cksum) {
        return NULL;
      }
      g_dbuffer.size = lz4_decompress(g_cbuffer.data, g_cbuffer.size,
                                      g_dbuffer.data, sizeof(g_dbuffer.data));
      return &g_dbuffer;
    }
    j += sizeof(header) + header.size;
  }
  return NULL;
}

static _Bool overwrite_lyrics(int off, int track, const CBuffer *comp) {
  TrackHeader header;
  header.magic = MY_MAGIC;
  header.cksum = djb2(comp->mem, sizeof(comp->mem));
  header.track = track;
  header.size = comp->size;
  header.enabled = 1;
  atrac_write_metadata(SEC_META_TSTMP, off, (uint8_t *)&header, sizeof(header));
  for (int i = 0; i < comp->size; i += 8) {
    if (i + 8 > comp->size) {
      atrac_write_metadata(SEC_META_TSTMP, off + sizeof(header) + i, comp->data + i, comp->size % 8);
      break;
    }
    atrac_write_metadata(SEC_META_TSTMP, off + sizeof(header) + i, comp->data + i, 8);
  }
  return true;
}

MY_API _Bool write_lyrics(int track, const CBuffer *comp) {
  TrackHeader header;
  int j = 0;
  while (j < SEC_SZ) {
    atrac_read_metadata(SEC_META_TSTMP, j, (uint8_t *)&header, sizeof(header));
    if (header.size > SEC_SZ - j - sizeof(header) ||
        header.size > sizeof(g_cbuffer.data)) {
      break;
    }
    if (header.magic != MY_MAGIC) {
      break;
    }
    if (header.enabled && header.track == track) {
      header.enabled = false;
      atrac_write_metadata(SEC_META_TSTMP, j, (uint8_t *)&header, sizeof(header));
    }
    j += sizeof(header) + header.size;
  }
  j = 0;
  while (j < SEC_SZ) {
    atrac_read_metadata(SEC_META_TSTMP, j, (uint8_t *)&header, sizeof(header));
    if (header.size > SEC_SZ - j - sizeof(header) ||
        header.size > sizeof(g_cbuffer.data)) {
      return overwrite_lyrics(j, track, comp);
    }
    if (header.magic != MY_MAGIC) {
      return overwrite_lyrics(j, track, comp);
    }
    if (!header.enabled && header.size <= comp->size) {
      return overwrite_lyrics(j, track, comp);
    }
    j += sizeof(header) + header.size;
  }
  return false;
}

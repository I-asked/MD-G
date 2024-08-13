#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SEC_TSTMP (0x2)
#define SEC_SZ (0x91c)
#define MY_MAGIC (0x1307)

#define D_SZ (4096)
#define C_SZ (2048)

[[gnu::naked]] void atrac_read(unsigned short sector_nr,
                               unsigned short byte_ofs, uint8_t *data,
                               unsigned int count) {}

[[gnu::naked]] void atrac_write(unsigned short sector_nr,
                                unsigned short byte_ofs, const uint8_t *data,
                                unsigned int count) {}

static unsigned int djb2(const uint8_t *buf, int sz) {
  unsigned int hash = 5381;
  for (int i = 0; i < sz; ++i) {
    hash *= 33;
    hash += buf[i];
  }
  return hash;
}

typedef struct [[gnu::packed, gnu::aligned(1)]] TrackHeader {
  unsigned short magic : 16;
  unsigned int cksum : 24;
  uint8_t track : 8;
  unsigned short size : 15;
  _Bool enabled : 1;
} TrackHeader;
_Static_assert(sizeof(TrackHeader) == 8, "TrackHeader");

static int uncompress(const uint8_t *data, size_t data_length, uint8_t *dest,
                      size_t dest_length) {
  size_t op = 0, ip = 0;
  for (;;) {
    uint8_t token = data[ip++];
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
    if (offset == 0) {
      return -1;
    }

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

typedef union {
  uint8_t mem[D_SZ + 2];
  struct [[gnu::packed, gnu::aligned(1)]] {
    unsigned short size;
    uint8_t data[D_SZ];
  };
} DBuffer;
static DBuffer g_dbuffer;
_Static_assert(sizeof(g_dbuffer.mem) ==
                   sizeof(g_dbuffer.data) + sizeof(g_dbuffer.size),
               "g_dbuffer");

typedef union {
  uint8_t mem[C_SZ + 2];
  struct [[gnu::packed, gnu::aligned(1)]] {
    unsigned short size;
    uint8_t data[C_SZ];
  };
} CBuffer;
static CBuffer g_cbuffer = {
    .size = 113,
    .data = {203, 0,   0,   0,   244, 60,  74,  97,  44,  32,  119, 105, 114,
             32,  114, 101, 105, 115, 101, 110, 32,  117, 109, 32,  100, 105,
             101, 32,  87,  101, 108, 116, 10,  69,  109, 111, 45,  71,  105,
             114, 108, 115, 32,  105, 109, 32,  112, 114, 105, 118, 97,  116,
             101, 32,  106, 101, 116, 10,  76,  105, 108, 97,  32,  99,  97,
             115, 104, 32,  105, 109, 32,  80,  114, 97,  100, 97,  32,  98,
             97,  103, 10,  75,  0,   247, 1,   104, 97,  98,  101, 110, 32,
             122, 117, 32,  118, 105, 101, 108, 32,  115, 119, 27,  0,   15,
             102, 0,   69,  80,  32,  115, 119, 97,  103}};
_Static_assert(sizeof(g_cbuffer.mem) ==
                   sizeof(g_cbuffer.data) + sizeof(g_cbuffer.size),
               "g_cbuffer");

const DBuffer *read_lyrics(int track) {
  TrackHeader header;
  int j = 0;
  while (j < SEC_SZ) {
    atrac_read(SEC_TSTMP, j, (uint8_t *)&header, sizeof(header));
    if (header.size > SEC_SZ - j - sizeof(header) ||
        header.size > sizeof(g_cbuffer.data)) {
      return NULL;
    }
    if (header.magic != MY_MAGIC) {
      return NULL;
    }
    if (header.track == track && header.enabled) {
      g_cbuffer.size = header.size;
      atrac_read(SEC_TSTMP, j + sizeof(header), g_cbuffer.data, g_cbuffer.size);
      if ((djb2(g_cbuffer.mem, sizeof(g_cbuffer.mem)) & 0xFFFFFF) !=
          header.cksum) {
        return NULL;
      }
      g_dbuffer.size = uncompress(g_cbuffer.data, g_cbuffer.size,
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
  atrac_write(SEC_TSTMP, off, (uint8_t *)&header, sizeof(header));
  atrac_write(SEC_TSTMP, off + sizeof(header), comp->data, comp->size);
  return true;
}

_Bool write_lyrics(int track, const CBuffer *comp) {
  TrackHeader header;
  int j = 0;
  while (j < SEC_SZ) {
    atrac_read(SEC_TSTMP, j, (uint8_t *)&header, sizeof(header));
    if (header.size > SEC_SZ - j - sizeof(header) ||
        header.size > sizeof(g_cbuffer.data)) {
      break;
    }
    if (header.magic != MY_MAGIC) {
      break;
    }
    if (header.enabled && header.track == track) {
      header.enabled = false;
      atrac_write(SEC_TSTMP, j, (uint8_t *)&header, sizeof(header));
    }
    j += sizeof(header) + header.size;
  }
  j = 0;
  while (j < SEC_SZ) {
    atrac_read(SEC_TSTMP, j, (uint8_t *)&header, sizeof(header));
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

void main(void) {
  const uint8_t *q =
      (const uint8_t
           *)"Ja, wir reisen um die Welt\nEmo-Girls im private jet\nLila cash "
             "im Prada bag\nJa, wir haben zu viel swag\nJa, wir reisen um die "
             "Welt\nEmo-Girls im private jet\nLila cash im Prada bag\nJa, wir "
             "haben zu viel swag";
  write_lyrics(0, &g_cbuffer);
  const DBuffer *l = read_lyrics(0);
  for (const uint8_t *p = q, *m = l->data; *p != '\0'; ++p, ++m) {
    if (*p != *m) {
      __asm__ __volatile__(R"(
      .thumb

      mov r0,#0
      bx r0
      )"); // lol
    }
  }
}

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "common.h"

#define MY_MAGIC (0x1307)

#define D_SZ (4096)
#define C_SZ (2048)

typedef union {
  uint8_t mem[D_SZ + 2];
  struct [[gnu::packed, gnu::aligned(1)]] {
    unsigned short size;
    uint8_t data[D_SZ];
  };
} DBuffer;

typedef union {
  uint8_t mem[C_SZ + 2];
  struct [[gnu::packed, gnu::aligned(1)]] {
    unsigned short size;
    uint8_t data[C_SZ];
  };
} CBuffer;

MY_API const DBuffer *read_lyrics(int track);

MY_API _Bool write_lyrics(int track, const CBuffer *comp);

extern CBuffer g_cbuffer;
extern DBuffer g_dbuffer;

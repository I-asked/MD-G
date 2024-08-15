#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include "lrc.h"

CBuffer g_cbuffer = {
    .size = 109,
    .data = {244, 60,  74,  97,  44,  32,  119, 105, 114, 32,  114, 101, 105,
             115, 101, 110, 32,  117, 109, 32,  100, 105, 101, 32,  87,  101,
             108, 116, 10,  69,  109, 111, 45,  71,  105, 114, 108, 115, 32,
             105, 109, 32,  112, 114, 105, 118, 97,  116, 101, 32,  106, 101,
             116, 10,  76,  105, 108, 97,  32,  99,  97,  115, 104, 32,  105,
             109, 32,  80,  114, 97,  100, 97,  32,  98,  97,  103, 10,  75,
             0,   247, 1,   104, 97,  98,  101, 110, 32,  122, 117, 32,  118,
             105, 101, 108, 32,  115, 119, 27,  0,   15,  102, 0,   69,  80,
             32,  115, 119, 97,  103}};
_Static_assert(sizeof(g_cbuffer.mem) ==
                   sizeof(g_cbuffer.data) + sizeof(g_cbuffer.size),
               "g_cbuffer");

DBuffer g_dbuffer;
_Static_assert(sizeof(g_dbuffer.mem) ==
                   sizeof(g_dbuffer.data) + sizeof(g_dbuffer.size),
               "g_dbuffer");

void start(void) {
  const uint8_t *q =
      (const uint8_t
           *)"Ja, wir reisen um die Welt\nEmo-Girls im private jet\nLila cash "
             "im Prada bag\nJa, wir haben zu viel swag\nJa, wir reisen um die "
             "Welt\nEmo-Girls im private jet\nLila cash im Prada bag\nJa, wir "
             "haben zu viel swag";
  write_lyrics(0, &g_cbuffer);
  const DBuffer *l = read_lyrics(0);

  for (const uint8_t *p = q, *m = l->data; *p != '\0'; ++p, ++m) {
    assert(*p == *m);
  }
}

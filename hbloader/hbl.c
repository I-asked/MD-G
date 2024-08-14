#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/md.h>

static size_t read(FILE *instance, char *bp, size_t n) { return 0; }

static size_t write(FILE *instance, const char *bp, size_t n) { return 0; }

static const struct File_methods vmt = {
    .write = write,
    .read = read,
};

FILE *const stdout = &(FILE){
    .vmt = &vmt,
};
FILE *const stderr = &(FILE){
    .vmt = &vmt,
};
FILE *const stdin = &(FILE){
    .vmt = &vmt,
};

[[noreturn, gnu::naked]] void __assert_fail(const char *a0, const char *a1,
                                            unsigned int a2) {
  __asm__ __volatile__(R"(
  mov r0,#0
  bx r0
  )");
  __builtin_unreachable();
}

inline static unsigned int djb2(const uint8_t *buf, int sz) {
  unsigned int hash = 5381;
  for (int i = 0; i < sz; ++i) {
    hash *= 33;
    hash += buf[i];
  }
  return hash;
}

struct [[gnu::packed, gnu::aligned(1)]] HBHeader {
  uint16_t magic : 16;
  uint16_t cksum : 6;
  uint16_t blksz : 10;
} g_hbHeader;

#define MY_MAGIC 0x1307
[[gnu::visibility("hidden"), noreturn]] void _boot(void) {
  void *pdec = (void *)0x2005500 + 0x1000;
  void *pcom = (void *)0x2008f00 - 0x400;
  int isrc = 0;
  for (;;) {
    atrac_read_metadata(3, isrc, (uint8_t *)&g_hbHeader, sizeof(g_hbHeader));
    if (g_hbHeader.magic != MY_MAGIC) {
      break;
    }
    isrc += sizeof(g_hbHeader);
    atrac_read_metadata(3, sizeof(g_hbHeader) + isrc, pcom, g_hbHeader.blksz);
    isrc += g_hbHeader.blksz;
    assert(g_hbHeader.cksum == (djb2(pcom, g_hbHeader.blksz) & 0x3f));
    int ret = lz4_decompress(pcom, g_hbHeader.blksz, pdec, pcom - pdec);
    assert(ret > 0);
    pdec += ret;
  }
  __asm__ __volatile__(R"(
  bx pc
  .word 0x2006501
  )");
  __builtin_unreachable();
}

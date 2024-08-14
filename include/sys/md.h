#pragma once

#include <stddef.h>
#include <stdint.h>

#define MD_HB_MAGIC 0x1307

void atrac_read_metadata(unsigned short sector_nr, unsigned short byte_ofs,
                         uint8_t *data, unsigned int count);

void atrac_write_metadata(unsigned short sector_nr, unsigned short byte_ofs,
                          const uint8_t *data, unsigned int count);

void atrac_read_contentdata(unsigned short sector_nr, unsigned short byte_ofs,
                            uint8_t *data, unsigned int count);

void atrac_write_contentdata(unsigned short sector_nr, unsigned short byte_ofs,
                             const uint8_t *data, unsigned int count);

void tron_dly_tsk(unsigned int t);

int lz4_decompress(const uint8_t *data, size_t data_length, uint8_t *dest,
                   size_t dest_length);

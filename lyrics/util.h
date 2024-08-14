#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

unsigned int djb2(const uint8_t *buf, int sz);

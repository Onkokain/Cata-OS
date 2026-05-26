#include "memory.h"
#include <stdint.h>

void  *memcpy(void  *dst, const void  *src, uint16_t n) {
  uint8_t  *u8Dst = (uint8_t  *)dst;
  const uint8_t  *u8Src = (const uint8_t  *)src;

  for (uint16_t i = 0; i < n; i++)
    u8Dst[i] = u8Src[i];

  return dst;
}

void  *memset(void  *ptr, int value, uint16_t num) {
  uint8_t  *u8ptr = (uint8_t  *)ptr;

  for (uint16_t i = 0; i < num; i++)
    u8ptr[i] = (uint8_t)value;
  return ptr;
}

int memcmp(const void  *ptr1, const void  *ptr2, uint16_t num) {
  const uint8_t  *u8ptr1 = (const uint8_t  *)ptr1;
  const uint8_t  *u8ptr2 = (const uint8_t  *)ptr2;

  for (uint16_t i = 0; i < num; i++)
    if (u8ptr1[i] != u8ptr2[i])
      return 1;
  return 0;
}

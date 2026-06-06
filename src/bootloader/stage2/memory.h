#pragma once
#include <stdint.h>
#define SEG(segoff) (segoff>>16)
#define OFF(segoff) (segoff&0xffff)
#define SEGOFF_LIN(segoff) ((SEG(segoff)<<4)+OFF(segoff))

void  *memcpy(void  *dst, const void  *src, uint16_t n);
void  *memset(void  *ptr, int value, uint16_t num);

int memcmp(const void  *ptr1, const void  *ptr2, uint16_t num);

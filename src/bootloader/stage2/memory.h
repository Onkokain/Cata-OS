#pragma once
#include "stdint.h"
#include "fardef.h"

void far* memcpy(void far* dst, const void far* src, uint16_t n);
void far* memset(void far* ptr, int value, uint16_t num);

int memcmp(const void far* ptr1, const void far* ptr2, uint16_t num);

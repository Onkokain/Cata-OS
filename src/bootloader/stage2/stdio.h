#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

void printf_signed(signed long long number, int basex);
void printf_unsigned(unsigned long long number, int basex);
void clrscr();
void putc(char c);
void puts(const char *str);
void printf(const char *fmt, ...);
void setcursor(int x, int y);

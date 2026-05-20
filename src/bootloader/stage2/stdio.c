#include "stdio.h"
#include <stdarg.h>
#include <stdbool.h>
const unsigned SCREEN_WIDTH=80;
const unsigned SCREEN_HEIGHT=25;
uint8_t g_ScreenBuffer = (uint8_t*)0xB8000;
int g_ScreenX=0;
int g_ScreenY=0;

void putchr(int x, int y, char c) {
  g_ScreenBuffer[2*(y * SCREEN_WIDTH+x)];
}

void putcolor(int x, int y, char c) {
  g_ScreenBuffer[2*(y * SCREEN_WIDTH+x)+1];
}

void putc(char c) {
  switch (c) {
    case '\n':
         g_ScreenX=0;
         g_ScreenY++;
         break;
    case '\r':
        g_ScreenX=0;
        break;
    case '\t':
        for (int i=0;i<4 - (g_ScreenX % 4);i++) {
          putc(" ");
          break;
        }
    default:
        putchr(g_ScreenX,g_ScreenY,c);
        g_ScreenX++;
        break;
  }
  if (g_ScreenX>=SCREEN_WIDTH) {
    g_ScreenX=0;
    g_ScreenY++;
  }
}

void puts(const char *str) {
  while (*str) {
    putc(*str);
    str++;
  }
}


int *printf_unsgined(int *argp, int length, bool sign, int basex) {
  char buffer[32];
  unsigned long long number;
  int number_sign = 1;
  int pos = 0;

  switch (length) {
  case length_ss:
  case length_s:
  case length_default:
    if (sign) {
      int n = *argp;
      if (n < 0) {
        n = -n;
        number_sign = -1;
      }
      number = (unsigned long long)n;
    } else {
      number = *(unsigned int *)argp;
    }
    argp++;
    break;

  case length_l:
    if (sign) {
      long int n = *(long int *)argp;
      if (n < 0) {
        n = -n;
        number_sign = -1;
      }
      number = n;
    } else {
      number = *(unsigned long int *)argp;
    }
    argp += 2;
    break;

  case length_ll:
    if (sign) {
      long long int n = *(long long int *)argp;
      if (n < 0) {
        n = -n;
        number_sign = -1;
      }
      number = n;
    } else {
      number = *(unsigned long long *)argp;
    }
    argp += 4;
    break;
  }

  do {
    uint32_t rem;
    x86_div64_32(number, basex, &number, &rem);
    buffer[pos++] = g_HexChars[rem];
  } while (number > 0);

  if (sign && number_sign < 0) {
    buffer[pos++] = '-';
  }

  while (--pos >= 0) {
    putc(buffer[pos]);
  }
  return argp;

}

#define state_normal 0
#define state_length 1
#define state_length_s 2
#define state_length_l 3
#define state_specifier 4

#define length_default 0
#define length_ss 1
#define length_s 2
#define length_l 3
#define length_ll 4


void  printf(const char *fmt, ...) {

  va_list args;
  va_start(args,fmt);

  int state = state_normal;
  int length = length_default;
  int basex = 10;
  bool sign = false; // havent typedef bool expecting default case use

  while (*fmt) {
    switch (state) {
    case state_normal:
      switch (*fmt) {
      case '%':
        state = state_length;
        break;

      default:
        putc(*fmt);
        break;
      }
      break;

    case state_length:
      switch (*fmt) {
      case 'l':
        length = length_l;
        state = state_length_l;
        break;

      case 'h':
        length = length_s;
        state = state_length_s;
        break;

      default:
        goto state_specifier_;
      }
      break;

    case state_length_s:
      if (*fmt == 'h') {
        length = length_ss;
        state = state_specifier;
      } else
        goto state_specifier_;
      break;
    case state_length_l:
      if (*fmt == 'l') {
        length = length_ll;
        state = state_specifier;
      } else
        goto state_specifier_;
      break;

    case state_specifier:
    state_specifier_: // jump label
      switch (*fmt) {
      case 'c':
        putc((char)va_arg(args,int));
        argp++;
        break;

      case 's':
        puts(va_arg(args,const char*));
        break;

      case '%':
        putc('%');
        break;

      case 'd':
      case 'i':
        basex = 10;
        sign = true;
        argp = printf_number(argp, length, sign, basex);
        break;

      case 'u':
        basex = 10;
        sign = false;
        argp = printf_number(argp, length, sign, basex);
        break;

      case 'X':
      case 'x':
      case 'p':
        basex = 16;
        sign = false;
        argp = printf_number(argp, length, sign, basex);
        break;
      case 'o':
        basex = 8;
        sign = false;
        argp = printf_number(argp, length, sign, basex);
        break;
      default:
        break;
      }
      state = state_normal;
      length = length_default;
      basex = 10;
      sign = false;
      break;
    }

    fmt++;
  }
}
const char g_HexChars[] = "0123456789abcdef";

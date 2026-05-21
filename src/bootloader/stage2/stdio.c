#include "stdio.h"
#include <stdarg.h>
#include <stdbool.h>
const unsigned SCREEN_WIDTH=80;
const unsigned SCREEN_HEIGHT=25;
const char g_HexChars[] = "0123456789abcdef";
uint8_t* g_ScreenBuffer = (uint8_t*)0xB8000;
int g_ScreenX=0;
int g_ScreenY=0;

void putchr(int x, int y, char c) {
  g_ScreenBuffer[2*(y * SCREEN_WIDTH+x)]=c;
}

void putcolor(int x, int y, char color) {
  g_ScreenBuffer[2*(y * SCREEN_WIDTH+x)+1]=color;
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
          putc(' ');
        }
        break;
    default:
        putchr(g_ScreenX,g_ScreenY,c);
        putcolor(g_ScreenX,g_ScreenY,0x0F);
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


void printf_unsigned(unsigned long long number, int basex) {
  char buffer[32];
  int pos = 0;
  // convert number to ascii code
  do {
    unsigned long long rem=number % basex;
    number/=basex;
    buffer[pos++]=g_HexChars[rem];
  } while (number > 0);
 // printing in reverse order
  while (--pos >= 0)
    putc(buffer[pos]);
}

void printf_signed(signed long long number, int basex) {
  if (number<0) {
    putc('-');
    printf_unsigned(-number,basex);
  }
  else printf_unsigned(number, basex);
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
  bool sign = false;
  bool isnumber=false;

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
        break;

      case 's':
        puts(va_arg(args,const char*));
        break;

      case '%':
        putc('%');
        break;

      case 'd':
      case 'i':
        isnumber=true;
        basex = 10;
        sign = true;
        break;

      case 'u':
        isnumber=true;
        basex = 10;
        sign = false;
        break;

      case 'X':
      case 'x':
      case 'p':
        isnumber=true;
        basex = 16;
        sign = false;
        break;
      case 'o':
        isnumber=true;
        basex = 8;
        sign = false;
        break;

      default:
        break;
      }
      if (isnumber) {
          if (sign) {
            switch (length) {
              case length_ss:
              case length_s:
              case length_default:
                  printf_signed(va_arg(args,int),basex);
                  break;

            case length_l:
                printf_signed(va_arg(args,long),basex);
                break;

            case length_ll:
                printf_signed(va_arg(args,long long),basex);
                break;
            }
          }
          else {
            switch (length) {
              case length_ss:
              case length_s:
              case length_default:
                  printf_unsigned(va_arg(args,unsigned int),basex);
                  break;

            case length_l:
                printf_unsigned(va_arg(args,unsigned long),basex);
                break;

            case length_ll:
                printf_unsigned(va_arg(args,unsigned long long),basex);
                break;
            }
          }
      }
      state = state_normal;
      length = length_default;
      basex = 10;
      sign = false;
      isnumber=false;
      break;
    }

    fmt++;
  }
  va_end(args);
}

#include "stdio.h"
#include "x86.h"

void putc(char c) {
  x86_Video_WriteCharTeletype(c,0);
}

void puts(const char* str) {
  while(*str) {
    putc(*str);
    str++;
  }
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

int* printf_number(int* argp,int length,bool sign, int basex)

void _cdecl printf(const char* fmt, ...) {
  int* argp=(int)&fmt;
  int state=state_normal;
  int length=length_default;
  int basex=10;
  bool signed= false; // havent typedef bool expecting default case use

  argp++;
  while(*fmt){
    switch (state)
    {
      case state_normal:
        switch(*fmt) {
          case "%":
          state=state_length;
          break;

          default:
          putc(*fmt);
          break;
        }
        break;

      case state_length:
        switch(*fmt) {
          case "l":
          length=length_l;
          state=state_length_l;
          case "h":
          length=length_s;
          state=state_length_s;
          default:
          goto state_specifier_;
        }
        break;

      case state_length_s:
        if (*fmt=='h') {
          length=length_ss;
          state=state_specifier;
        }
        else goto state_specifier_;
        break;
      case state_length_l:
        if (*fmt=='l') {
          length=length_ll;
          state=state_specifier;
        }
        else goto state_specifier_;
        break;

      case state_specifier:
      state_specifier_: // jump label
        switch (*fmt) {
          case "c":
          putc((char)*argp);
          argp++;
          break;

          case "s":
          puts((char)*argp);
          argp++;
          break;

          case "%":
          putc("%");
          break;

          case "d":
          case "i":
          basex=10;
          sign=true;
          argp=printf_number(argp,length,sign,basex);

          case "u":
          basex=10;
          sign=false;
          argp=printf_number(argp,length,sign,basex);
          case "X":
          case "x":
          case "p":
          basex=16;
          sign=false;
          argp=printf_number(argp,length,sign,basex);

          case "o":
          basex=8;
          sign=true;
          argp=printf_number(argp,length,sign,basex);

          default:
          break;
        }
        state=state_normal;
        length=length_default;
        basex=10
        sign=false;
        break;
    }

    fmt++;
  }
}
const char g_HexChars[]="0123456789abcdef";
int* printf_number(int* argp,int length,bool sign, int basex){
  char buffer[32];
  unsigned long long number;
  int number_sign=1;
  int pos=0;

  switch (length) {
    case length_ss:
    case length_s:
    case length_default:
      if (sign) {
        int n=*argp;
        if (n<0) {
          n=-n;
        }
      }
  }

}

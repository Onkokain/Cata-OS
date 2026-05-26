#include "ctype.h"

char tolower(char chr) {
  if (isupper(chr))
    return chr - 'A' + 'a';
  return chr;
}

char toupper(char chr) {
  if (islower(chr))
    return chr - 'a' + 'A';
  return chr;
}

bool isupper(char chr) {
  if (chr >= 'A' && chr <= 'Z')
    return true;
  else
    return false;
}
bool islower(char chr) {
  if (chr >= 'a' && chr <= 'z')
    return true;
  else
    return false;
}

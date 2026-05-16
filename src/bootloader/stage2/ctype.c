#include "ctype.h"


char tolower(char chr){
  return isupper(chr) ? (chr-'A'+'a'): chr;

}

char toupper(char chr){
  return islower(chr) ? (chr-'a'+'A'): chr;
}

bool isupper(char chr){
  return chr>='A' && chr<='Z';

}
bool islower(char chr){
  return chr>='a' && chr<='z';
}

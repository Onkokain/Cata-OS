#include "stdint.h"
#include "stdio.h"

void _cdecl cstart_(uint16_t bootDrive){
  puts("Hello World from C!");
  printf("\nPrintf should lowkey work rn..");
  printf("\nif it doesn't fml..");
  for (;;);
}

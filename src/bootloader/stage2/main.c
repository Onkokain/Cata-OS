#include "stdint.h"
#include "stdio.h"
#include "disk.h"
#include "fat.h"

void _cdecl cstart_(uint16_t bootDrive){
  DISK disk;
  if (!DISK_Initiaize(&disk, bootDrive)) {
    printf("disk init failed..\n");
    goto end;
  }
  if (!FAT_Init(&disk)) {
    printf("FAT init failed..\n");
    goto end;
  }


  end:
    while (1) {};
}

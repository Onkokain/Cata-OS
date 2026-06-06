#include <stdint.h>
#include "stdio.h"
#include "x86.h"
#include "disk.h"
#include "fat.h"
#include "memodef.h"
#include "memory.h"
#include "vbe.h"

uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_KERNEL_ADDR;
uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_STACK_ADDR;

typedef void (*KernelStart)();

void __attribute((cdecl)) start(uint16_t bootDrive) {
  clrscr();
  DISK disk;
  if (!DISK_init(&disk, bootDrive)) {
    printf("disk init failed..\n");
    goto end;
  }
  if (!FAT_Init(&disk)) {
    printf("FAT init failed..\n");
    goto end;
}
  // load kernel

  FAT_File *fd = FAT_Open(&disk, "/kernel.bin");
  uint32_t read;
  uint8_t *kernelBuffer = Kernel;
  while ((read= FAT_Read(&disk, fd, MEMORY_KERNEL_SIZE, KernelLoadBuffer))) {

    printf("Memcpy works.. while loop passed..\n");

    memcpy(kernelBuffer,KernelLoadBuffer, read );
    kernelBuffer+=read;
  }
  printf("while loop exited..\n");
  FAT_Close(fd);

  const int desiredW = 1024;
  const int desiredH = 768;
  const int desiredBpp = 32;
  uint16_t pickedMode=0xffff;
  printf("graphics initialized...\n");

  // init graphics
  VbeInfoBlock* info=(VbeInfoBlock*)MEMORY_VESA_INFO;
  VbeModeInfo* modeInfo=(VbeModeInfo*)MEMORY_MODE_INFO;

  if (VBE_GetControllerInfo(info)) {
    printf("Controller info received...\n");
    uint16_t* mode = (uint16_t*)(info->VideoModePtr);
    printf("this is properly executed");
    for (int i=0; mode[i]!=0xFFFF; i++) {
      if (!VBE_GetModeInfo(mode[i], modeInfo)) {
          printf("Can't get mode info %x :(\n", mode[i]);
          continue;
      }
      printf("anything below this (including this) is not being executed why??");
      bool hasFB = (modeInfo->attributes & 0x90) == 0x90;

      if (hasFB && modeInfo->width == desiredW && modeInfo->height == desiredH && modeInfo->bpp == desiredBpp) {
          pickedMode = mode[i];
          break;
      }
    }
    if (pickedMode!=0xffff && VBE_SetModeInfo(pickedMode)) {
      // printf("Mode %x selected!\n", pickedMode);
      printf("4");

      uint32_t* fb=(uint32_t*)(modeInfo->framebuffer);
      int w=modeInfo->width;
      int h=modeInfo->height;
      for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
          fb[y*modeInfo->pitch+x]=x+y;
        }
      }
    }
  }
  else {
    printf("No VBE extensions \n");
  }

  // execute kernel
  // KernelStart kernelStart= (KernelStart)Kernel;
  // kernelStart();
end:
  while (1) {};
}











































//   DISK disk;
//   if (!DISK_init(&disk, bootDrive)) {
//     printf("disk init failed..\n");
//     goto end;
//   }
//   if (!FAT_Init(&disk)) {
//     printf("FAT init failed..\n");
//     goto end;
// }
//   // browse

//   FAT_File *fd = FAT_Open(&disk, "/");
//   FAT_DirectoryEntry entry;
//   int i = 0;
//   while (FAT_ReadEntry(&disk, fd, &entry) && i++ < 5) {
//     printf("  ");
//     for (int i = 0; i < 11; i++)
//       putc(entry.Name[i]);
//     printf("\r\n");
//   }
//   FAT_Close(fd);

//   // read test.txt
//   char buffer[100];
//   uint32_t read;
//   fd = FAT_Open(&disk, "teast.txt"); // WILL FAIL IF FOLDER DOESNT EXIST #fix
//   if(!fd){
//     printf("Reading failed!");
//     goto end;
//   }
//   while ((read = FAT_Read(&disk, fd, sizeof(buffer), buffer))) {
//     for (uint32_t i = 0; i < read; i++) {
//       if (buffer[i] == '\n')
//         putc('\r');
//       putc(buffer[i]);
//     }
//   }
//   FAT_Close(fd);

// end:
//   while (1) {
//   };

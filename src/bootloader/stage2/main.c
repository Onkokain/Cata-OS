#include <stdint.h>
#include "stdio.h"
#include "x86.h"
#include "disk.h"
#include "fat.h"
#include "memodef.h"
#include "memory.h"

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
    memcpy(kernelBuffer,KernelLoadBuffer, read );
    kernelBuffer+=read;
  }
  FAT_Close(fd);

  //execute kernel
  KernelStart kernelStart= (KernelStart)Kernel;
  kernelStart();
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

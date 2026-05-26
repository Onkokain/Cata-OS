#include <stdint.h>
#include "stdio.h"
#include "x86.h"
#include "disk.h"

void *g_data = (void *)0x20000;

// void puts_realmode(const char* str) {
//   while (*str) {
//     RealMode_Putc(*str);
//     ++str;
//   }
// }

void __attribute__((cdecl)) start(uint16_t bootDrive) {
  // uint8_t driveType;
  // uint16_t cylinders;
  // uint16_t sectors;
  // uint16_t heads;

  // x86_Disk_GetDriveParameters(bootDrive, &driveType, &cylinders, &sectors, &heads);
  // printf("drivetype:%u \n cyls=%u \n sectors=%u \n heads=%u \n",driveType,cylinders,sectors,heads);
  clrscr();
  DISK disk;
  if (!DISK_init(&disk, bootDrive)) {
    printf("disk init failed..\n");
    goto end;
  }
  DISK_ReadSectors(&disk, 0, 1, g_data);




end:
for (;;);
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

//   FAT_File far *fd = FAT_Open(&disk, "/");
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

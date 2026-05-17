#include "disk.h"
#include "fat.h"
#include "stdint.h"
#include "stdio.h"

void far *g_data = (void far *)0x00500200;

void _cdecl cstart_(uint16_t bootDrive) {
  DISK disk;
  if (!DISK_init(&disk, bootDrive)) {
    printf("disk init failed..\n");
    goto end;
  }
  if (!FAT_Init(&disk)) {
    printf("FAT init failed..\n");
    goto end;
  }
  // browse

  FAT_File far *fd = FAT_Open(&disk, "/");
  FAT_DirectoryEntry entry;
  int i = 0;
  while (FAT_ReadEntry(&disk, fd, &entry) && i++ < 5) {
    printf("  ");
    for (int i = 0; i < 11; i++)
      putc(entry.Name[i]);
    printf("\r\n");
  }
  FAT_Close(fd);

  // read test.txt
  char buffer[100];
  uint32_t read;
  fd = FAT_Open(&disk, "test.txt"); // WILL FAIL IF FOLDER DOESNT EXIST #fix
  while ((read = FAT_Read(&disk, fd, sizeof(buffer), buffer))) {
    for (uint32_t i = 0; i < read; i++) {
      if (buffer[i] == '\n')
        putc('\r');
      putc(buffer[i]);
    }
  }
  FAT_Close(fd);

end:
  while (1) {
  };
}

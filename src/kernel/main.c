#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include "hal/hal.h"
#include <arch/i686/irq.h>
extern uint8_t __bss_start;
extern uint8_t __end;

void crash_me();

void timer(Registers* regs) {
  printf("-");
}

void __attribute((section(".entry"))) start(uint16_t bootDrive) {
  memset(&__bss_start, 0, (&__end) - (&__bss_start));
  HAL_Initialize();
  clrscr();
  printf("Hello world from kernel!!\n");

  i686_IRQ_RegisterHandler(0, timer);

  // crash_me();
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

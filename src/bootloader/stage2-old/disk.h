#pragma once
#include "fardef.h"
#include "stdint.h"

typedef struct {
  uint8_t id;
  uint16_t cylinders;
  uint16_t sectors;
  uint16_t heads;

} DISK;

bool DISK_init(DISK *disk, uint8_t driveNumber);

bool DISK_ReadSectors(DISK *disk, uint32_t lba, uint8_t sectors,
                      void far *dataOut);

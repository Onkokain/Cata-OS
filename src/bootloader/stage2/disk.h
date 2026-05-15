#pragma once

#include "stdint.h"

typedef struct (
  uint8_t id;
  uint16_t cylinders;
  uint16_t sectors;
  uint16_t heads;

) DISK;

bool DISK_init(DISK* disk, uint8_t driveNumber);

bool DISK_ReadSecctors(DISK* disk, uint32_t lba, uint8_t sectors, uint8_t far* dataOut);




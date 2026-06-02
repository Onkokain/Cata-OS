#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  const char* (Name);
  bool (*Probe)(void);
  void (*Initialize)(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEOI);
  void (*Disable)(void);
  void (*SendEOI)(int irq);
  void (*Mask)(int irq);
  void (*UnMask)(int irq);

} PICDriver;

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "pic.h"

PICDriver* i8259_GetDriver();


void i8259_Configure(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEOI);

void i8259_SendEOI(int irq);
void i8259_Disable();
void i8259_Unmask(int irq);
void i8259_Mask(int irq);
uint16_t i8259_ReadIRQRequestRegister();
uint16_t i8259_ReadInServiceRegister();
void i8259_SetMask(uint16_t mask);

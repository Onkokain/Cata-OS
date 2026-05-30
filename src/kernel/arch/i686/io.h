#pragma once
#include <stdint.h>
#include <stdbool.h>
void __attribute__((cdecl)) x86_outb(uint16_t port, uint8_t value);
uint8_t __attribute__((cdecl)) x86_inb(uint16_t port);

uint8_t __attribute__((cdecl)) x86_EnableInterrupts();
uint8_t __attribute__((cdecl)) x86_DisableInterrupts();

void __attribute__((cdecl)) i686_panic();

void i686_iowait();


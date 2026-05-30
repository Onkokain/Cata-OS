#pragma once
#include <stdint.h>
typedef struct {
  uint32_t ds;
  uint32_t edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
  uint32_t interrupt, error;
  uint32_t eip, cs, eflags, esp, ss;

} __attribute((packed)) Registers;

typedef void (*ISRHandler)(Registers* regs);

void __attribute__((cdecl)) i686_ISR_Handler(Registers* regs);
void i686_ISR_Initialize();

void i686_ISR_RegisterHandler(int interrupt, ISRHandler handler);



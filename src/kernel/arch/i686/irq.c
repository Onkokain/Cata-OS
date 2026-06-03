#include "irq.h"
#include "i8259.h"
#include "io.h"
#include "stdio.h"
#include <stdbool.h>
#include <util/arrays.h>

#define PIC_REMAP_OFFSET 0x20

IRQHandler g_IRQHandlers[16];

static PICDriver* g_Driver= NULL;

void i686_IRQ_Handler(Registers* regs) {
  int irq = regs->interrupt - PIC_REMAP_OFFSET;

  if (g_IRQHandlers[irq] != NULL) {
    g_IRQHandlers[irq](regs);
  }
  else {
    printf("Unhandled IRQ %d....\n", irq);
  }

  // i8259_SendEOI(irq);
  g_Driver->SendEOI(irq);
}

void i686_IRQ_Init() {

  PICDriver* drivers[]= {
    i8259_GetDriver(),
  };

  for (int i=0;i<SIZE(drivers);i++) {
    if (drivers[i]->Probe()) {
      g_Driver = drivers[i];
    }
  }
  if (g_Driver == NULL) {
    printf("No PIC driver found\n");
    return;
  }
  printf("Using PIC driver: %s\n", g_Driver->Name);

  g_Driver->Initialize(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
  // i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);

  for (int i=0;i<16;i++) {
    i686_ISR_RegisterHandler(PIC_REMAP_OFFSET + i, i686_IRQ_Handler);
  }
  x86_EnableInterrupts();

  g_Driver->UnMask(0);  
  g_Driver->UnMask(1);
}
void i686_IRQ_RegisterHandler(int irq, IRQHandler handler) {
  g_IRQHandlers[irq] = handler;
}

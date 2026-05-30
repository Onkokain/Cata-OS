#include "isr.h"
#include "gdt.h"
#include <stdio.h>
#include "idt.h"
#include <stddef.h>
#include "io.h"

ISRHandler g_ISR_Handlers[256];

static const char* const g_Exceptions[] = {
    "\nDivide by zero error",
    "\nDebug",
    "\nNon-maskable Interrupt",
    "\nBreakpoint",
    "\nOverflow",
    "\nBound Range Exceeded",
    "\nInvalid Opcode",
    "\nDevice Not Available",
    "\nDouble Fault",
    "\nCoprocessor Segment Overrun",
    "\nInvalid TSS",
    "\nSegment Not Present",
    "\nStack-Segment Fault",
    "\nGeneral Protection Fault",
    "\nPage Fault",
    "\n",
    "\nx87 Floating-Point Exception",
    "\nAlignment Check",
    "\nMachine Check",
    "\nSIMD Floating-Point Exception",
    "\nVirtualization Exception",
    "\nControl Protection Exception ",
    "\n",
    "\n",
    "\n",
    "\n",
    "\n",
    "\n",
    "\nHypervisor Injection Exception",
    "\nVMM Communication Exception",
    "\nSecurity Exception",
    "\n"
};

void  i686_ISR_InitializeGates();

void i686_ISR_Initialize() {
  i686_ISR_InitializeGates();
  for (int i=0; i<256;i++)
    i686_IDT_EnableGate(i);

  i686_IDT_DisableGate(0x80);
}

void __attribute__((cdecl)) i686_ISR_Handler(Registers* regs) {
  // printf("Received interrupt: %d\n", regs->interrupt);
  if (g_ISR_Handlers[regs->interrupt] != NULL)
    g_ISR_Handlers[regs->interrupt](regs);
  else if (regs->interrupt >= 32)
    printf("Unhandled interrupt: %d\n \n", regs->interrupt);
  else {
    printf("Unhandled exception: %d %s\n", regs->interrupt, g_Exceptions[regs->interrupt]);

    printf("eax=%x ebx=%x ecx=%x edx=%x esi=%x edi=%x\n",regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi);

    printf("esp=%x ebp=%x eip=%x eflags=%x ds=%x ss=%x\n",regs->esp, regs->ebp,regs->eip, regs->eflags, regs->ds, regs->ss);

    printf("interrupt=%x errcode=%x\n", regs->interrupt, regs->error);

    printf("KERNEL PANIC!!!\n");
    i686_panic();
  }
}

void i686_ISR_RegisterHandler(int interrupt, ISRHandler handler) {
  g_ISR_Handlers[interrupt] = handler;
  i686_IDT_EnableGate(interrupt);
}


#include "idt.h"
#include <stdint.h>
#include <util/binary.h>
typedef struct {
  uint16_t baselow;
  uint16_t segselector;
  uint8_t reserved;
  uint8_t flags;
  uint16_t basehigh;
} __attribute((packed)) IDTEntry;

typedef struct {
  uint16_t limit;
  uint32_t Ptr;
} __attribute__((packed)) IDTDescriptor;



IDTEntry g_IDT[256];

IDTDescriptor g_IDTDescriptor = { sizeof(g_IDT) - 1, (uint32_t)g_IDT };


void __attribute__((cdecl)) i686_IDT_Load(IDTDescriptor* idtDescriptor);

void i686_IDT_SetGate(int interrupt, void* base,uint16_t segmentDescriptor, uint8_t flags)
{
  g_IDT[interrupt].baselow = ((uint32_t)base) & 0xFFFF;
  g_IDT[interrupt].segselector = segmentDescriptor;
  g_IDT[interrupt].reserved = 0;
  g_IDT[interrupt].flags = flags;
  g_IDT[interrupt].basehigh = ((uint32_t)base>>16) & 0xFFFF;
}

void i686_IDT_EnableGate(int interrupt) {
  FLAG_SET(g_IDT[interrupt].flags, IDT_FLAG_PRESENT);

}

void i686_IDT_DisableGate(int interrupt) {
  FLAG_UNSET(g_IDT[interrupt].flags, IDT_FLAG_PRESENT);

}

void i686_IDT_Initialize()
{
  i686_IDT_Load(&g_IDTDescriptor);
}


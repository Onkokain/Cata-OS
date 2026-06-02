#include "i8259.h"
#include "io.h"
#include <stdbool.h>

#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1

enum {
  PIC_ICW1_ICW4 = 0x01,
  PIC_ICW1_SINGLE = 0x02,
  PIC_ICW1_INTERVAL4 = 0x04,
  PIC_ICW1_LEVEL = 0x08,
  PIC_ICW1_INITIALIZE = 0x10,

} PIC_ICW1;

enum {
  PIC_ICW4_8086 = 0x1,
  PIC_ICW4_AUTO_EOI = 0x2,
  PIC_ICW4_BUFFERED_MASTER = 0x4,
  PIC_ICW4_BUFFERED_SLAVE = 0x0,
  PIC_ICW4_BUFFERED = 0x8,
  PIC_ICW4_SFNM = 0x10,
} PIC_ICW4;

enum {
  PIC_CMD_EOI = 0x20,
  PIC_CMD_READ_IRR = 0x0A,
  PIC_CMD_READ_ISR = 0x0B,

} PIC_CMD;

static uint16_t g_PicMask = 0xffff;
static bool g_AutoEOI= false;

void i8259_Configure(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEOI) {

  i8259_SetMask(0xFFFF);

  x86_outb(PIC1_COMMAND_PORT, PIC_ICW1_INITIALIZE | PIC_ICW1_ICW4);
  i686_iowait();
  x86_outb(PIC2_COMMAND_PORT, PIC_ICW1_INITIALIZE | PIC_ICW1_ICW4);
  i686_iowait();

  x86_outb(PIC1_DATA_PORT, offsetPic1);
  i686_iowait();
  x86_outb(PIC2_DATA_PORT, offsetPic2);
  i686_iowait();

  x86_outb(PIC1_DATA_PORT, 0x04);
  i686_iowait();
  x86_outb(PIC2_DATA_PORT, 0x02);
  i686_iowait();

  x86_outb(PIC1_DATA_PORT, PIC_ICW4_8086);
  i686_iowait();
  x86_outb(PIC2_DATA_PORT, PIC_ICW4_8086);
  i686_iowait();


  uint8_t icw4 = PIC_ICW4_8086;
  if (autoEOI) {
    icw4|=PIC_ICW4_AUTO_EOI;
  }
  x86_outb(PIC1_DATA_PORT, icw4);
  i686_iowait();
  x86_outb(PIC2_DATA_PORT, icw4);
  i686_iowait();


  i8259_SetMask(0xFFFF);
}

void i8259_SendEOI(int irq){
  if (irq>=8)
      x86_outb(PIC2_COMMAND_PORT, PIC_CMD_EOI);

  x86_outb(PIC1_COMMAND_PORT, PIC_CMD_EOI);
}


void i8259_Disable() {
  i8259_SetMask(0xFFFF);
}

void i8259_SetMask(uint16_t mask) {
  g_PicMask = mask;
  x86_outb(PIC1_DATA_PORT, g_PicMask & 0xFF);
  i686_iowait();
  x86_outb(PIC2_DATA_PORT, g_PicMask >> 8);
  i686_iowait();
}

uint16_t i8259_GetMask() {

  return x86_inb(PIC1_DATA_PORT) | (x86_inb(PIC2_DATA_PORT)<<8);
}

void i8259_Mask(int irq) {
  i8259_SetMask(g_PicMask | (1<<irq));
}

void i8259_Unmask(int irq) {
  i8259_SetMask(g_PicMask &  ~(1<<irq));
}

uint16_t i8259_ReadIRQRequestRegister() {
  x86_outb(PIC1_COMMAND_PORT, PIC_CMD_READ_IRR);
  x86_outb(PIC2_COMMAND_PORT, PIC_CMD_READ_IRR);
  return x86_inb(PIC2_COMMAND_PORT) | (x86_inb(PIC2_COMMAND_PORT) << 8);
}
uint16_t i8259_ReadInServiceRegister() {
  x86_outb(PIC1_COMMAND_PORT, PIC_CMD_READ_ISR);
  x86_outb(PIC2_COMMAND_PORT, PIC_CMD_READ_ISR);
  return x86_inb(PIC2_COMMAND_PORT) | (x86_inb(PIC2_COMMAND_PORT) << 8);
}

bool i8259_Probe() {

  i8259_Disable();
  i8259_SetMask(0x1337);
  return i8259_GetMask() == 0x1337;

}

static const PICDriver  g_PicDriver= {
  .Name="8259 PIC",
  .Probe = &i8259_Probe,
  .Initialize = &i8259_Configure,
  .Disable = &i8259_Disable,
  .Mask = &i8259_Mask,
  .UnMask = &i8259_Unmask,
  .SendEOI = &i8259_SendEOI,
};

PICDriver* i8259_GetDriver() {
  return (PICDriver*)&g_PicDriver;
}


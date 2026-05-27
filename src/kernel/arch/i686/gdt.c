#include "gdt.h"
#include <stdint.h>
typedef struct {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t limit_high;
  uint8_t base_high;
} __attribute__((packed)) GDTEntry;

typedef struct {
  uint16_t limit;
  uint32_t ptr;
} __attribute__((packed)) GDTDescriptor;

typedef enum {
  GDT_ACCESS_CODE_READABLE = 0x02,
  GDT_ACCESS_DATA_WRITABLE = 0x02,

  GDT_ACCESS_CODE_CONFORMING = 0x04,
  GDT_ACCESS_DATA_DIRECTION_NORMAL = 0x00,
  GDT_ACCESS_DATA_DIRECTION_DOWN = 0x04,

  GDT_ACCESS_DATA_SEGMENT = 0x10,
  GDT_ACCESS_CODE_SEGMENT = 0x18,

  GDT_ACCESS_DESCRIPTOR_TSS = 0x00,

  GDT_ACCESS_RING0 = 0x00,
  GDT_ACCESS_RING1 = 0x20,
  GDT_ACCESS_RING2 = 0x40,
  GDT_ACCESS_RING3 = 0x60,

  GDT_ACCESS_PRESENT = 0x80,
} GDT_ACCESS;


typedef enum {
  GDT_FLAGS_64BIT = 0x20,
  GDT_FLAGS_32BIT = 0x40,
  GDT_FLAGS_16BIT = 0x00,

  GDT_FLAGS_GRANULARITY_1B = 0x00,
  GDT_FLAGS_GRANULARITY_4KB = 0x80,
} GDT_FLAGS;

//macros
#define GDT_LIMIT_LOW(limit) (limit & 0xFFFF)
#define GDT_BASE_LOW(base) (base & 0xFFFF)
#define GDT_BASE_MIDDLE(base) ((base >> 16) & 0xFF)
#define GDT_LIMIT_HIGH(limit, flags) ((((limit) >> 16) & 0xF) | ((flags) & 0xF0))

#define GDT_BASE_HIGH(base) ((base >> 24) & 0xFF)


#define GDT_ENTRY(base, limit, access, flags) { \
  GDT_LIMIT_LOW(limit), \
  GDT_BASE_LOW(base), \
  GDT_BASE_MIDDLE(base), \
  access, \
  GDT_LIMIT_HIGH(limit,flags), \
}

GDTEntry g_GDT[] = {
  GDT_ENTRY(0, 0, 0, 0),

GDT_ENTRY(
              0,
              0xFFFFFF,
              GDT_ACCESS_PRESENT |
              GDT_ACCESS_RING0 |
              GDT_ACCESS_CODE_SEGMENT |
              GDT_ACCESS_CODE_READABLE,

              GDT_FLAGS_32BIT |
              GDT_FLAGS_GRANULARITY_4KB
),

  GDT_ENTRY(
            0,
            0xFFFFFF,
            GDT_ACCESS_PRESENT |
            GDT_ACCESS_RING0 |
            GDT_ACCESS_CODE_SEGMENT |
            GDT_ACCESS_DATA_WRITABLE,
            GDT_FLAGS_32BIT |
            GDT_FLAGS_GRANULARITY_4KB),
};


GDTDescriptor g_GDTDescriptor = {sizeof(g_GDT)-1, (uint32_t)g_GDT};

void __attribute((cdecl)) i686_GDT_Load(GDTDescriptor* Descriptor, uint16_t codeSegment, uint16_t dataSegment);

void i686_GDT_Initialize()
{
  i686_GDT_Load(&g_GDTDescriptor, i686_GDT_CODE_SEGMENT,i686_GDT_DATA_SEGMENT );
}

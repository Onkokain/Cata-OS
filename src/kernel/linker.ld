ENTRY(start)
OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)

SECTIONS
{
  . = 0x00100000; /*phyiscal address*/

  .entry : {__entry_start = .; *(.entry)}
  .text  : {__text_start = .;  *(.text)}
  .data  : {__data_start = .;  *(.data)}
  .rodata  : {__rodata_start = .;  *(.rodata)}
  .bss : {__bss_start = .; *(.bss)}
  __end = .;
}

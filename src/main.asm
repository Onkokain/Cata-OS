org 0x7C00 ; bootsector where bios searches for os, 0x7c00 is for legacy booting

bits 16 ; emits 16 bits code, directive not instruction

main:
  hlt
.halt:
  jmp .halt

times 510-($-$$) db 0 ; makes the number of bits =512
dw 0AA55h ; for bootloader, dw=more than a single character


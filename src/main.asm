; bootsector is defined so that the bios can find the operating system at address 0x7C00, org is a directive to initiate the starting
org 0x7C00

bits 16 ; emits 16 bits code, directive not instruction

; main program loop starts
main:
  hlt
.halt:
  jmp .halt
; main program loop ends

times 510-($-$$) db 0 ; makes the number of bits =512 since using a floppy to boot

dw 0AA55h ; for bootloader, dw=more than a single character


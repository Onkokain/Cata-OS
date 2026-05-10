org 0
bits 16

%define ENDL 0x0D, 0x0A

main:
  ; mov ax,0
  ; mov ds,ax
  ; mov es,ax

  ; mox ss,ax
  ; mov sp, 0x7BFF ; stack grows downwards from init but with padding

  mov si, msg_kernel
  call prints

.halt:
  cli
  hlt
  jmp .halt

;
; Prints a string to the screen
;
prints:
  ; save registers to be modified later
  push si ; source index register 16bits pointer register
  push ax ; accumulator register 16bits general purpose register
;
; main print loop
;
.loop:
  lodsb ; loads the byte (one character at a time) at [DS:SI] into AL then moves it to SI
  or al,al ; verifies if current byte is null or not by doing a bitwise or inbetween al and al
  jz .done ; conditional jump if al==0 aka no more characters left to read
; if there are still characters left to read
  mov ah, 0x0e ; moves 0x0e (teletype output) to ah
  mov bh,0 ; moves 0 to bh (makes default page to display be 0)
  int 0x10 ; initializes bios video interupt aka prints the character
  jmp .loop ; loops if more strings left
;
; jump to done after print loop is done
;
.done:
  pop ax ; removing the defined in prints in reverse order
  pop si
  ret
;

msg_kernel: db "Hello world from kernel!", ENDL,0

times 510-($-$$) db 0 ; makes the number of bytes =512 since using a floppy to boot
dw 0AA55h ; for bootloader, dw= define words; 2 bytes

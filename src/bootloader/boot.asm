; bootsector defines the starting address as 0x7C00, org is a directive to initiate the starting; legacy booting
org 0x7C00
bits 16 ; emits 16 bits code, directive not instruction

 ;macro defined for a '\n' new line
%define ENDL 0x0D,0x0A

start:
  jmp main ; jumping to main since prints preceeds main
;
; Prints a string to the screen
;
prints: ; function to display text on the screen
  ; save registers to be modified
  push si
  push ax

.loop:
  lodsb ; loads next character into als
  or al,al ; verifies if next character is null
  jz .done ; conditional jump if bitwise or returns 0

  mov ah, 0x0e
  mov bh,0 ; moves bh to 0
  int 0x10
  jmp .loop ; loops if more strings left


.done:
  pop ax ; removing the defined in prints in reverse order
  pop si
  ret




; main program loop starts
main:
  ; data segments
  mov ax,0 ; using ax as a temp constant since cant write 0 directly to ds in 16 bit
  mov ds, ax
  mov es, ax

  ; stack segments
  mov ss,ax
  mov sp, 0x7c00 ; stack grows downwards from init

  ; prints message
  mov si,msg_hello
  mov si,hello
  call prints
  hlt


.halt:
  jmp .halt
; main program loop ends

msg_hello: db "Hello World!", ENDL,0
hello: db "Hello....", ENDL,0



times 510-($-$$) db 0 ; makes the number of bytes =512 since using a floppy to boot

dw 0AA55h ; for bootloader, dw= define word; 2 bytes

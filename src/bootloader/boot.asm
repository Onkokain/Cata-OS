; org is a 'directive' that defines where the code starts from, 0x7C00 is the default starting position on the disk
org 0x7C00
bits 16 ; directive that tells the assembler to generate code assuming 16-bit cpu mode
;macro defined for a '\n' new line
; syntax: %define <label>, [value1,value2,value3]
%define ENDL 0x0D,0x0A ; replaces ENDL anywhere on the code with 0x0D,0x0A
jmp short start ; short defines that the jmp is short and uses less bytes of space than just 'jump start'
nop ; does nothing used to fill space
;
; Fat12 header
; bdb= BIOS Data Block
bdb_oem: db 'MSWIN4.1'
bdb_bytes_per_sector: dw 512
bdb_sectors_per_cluster: db 1
bdb_reserved_sectors: dw 1
bdb_fat_count: db 2 ; 2 for redundancy
bdb_dir_entries_count: dw 0E0h
bdb_total_sectors: dw 2880
bdb_media_descriptor_type: db 0F0h
bdb_sectors_per_fat: dw 9
bdb_sectors_per_track: dw 18
bdb_heads: dw 2
bdb_hidden_sectors: dd 0
bdb_large_sector_count: dd 0
;
; extended boot record
;
ebr_drive_number: db 0
db 0 ; reserved empty byte
ebr_signature: db 29h
ebr_volume_id: db 12h, 45h, 35h, 67h ; random serial number for volume id
ebr_volume_label: db 'Barale OS  ' ; padded to 11 chars
ebr_system_id: db 'FAT12   ' ; padded to 8 chars
;
; main start label, only use is to bypass everything and jump to main
;
start:
  jmp main
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
; main program starts
;
main:
  ; data segments
  mov ax,0 ; using ax as a temp constant since cant write 0 directly to ds in 16 bit
  mov ds, ax
  mov es, ax
  ; stack segments
  mov ss,ax
  mov sp, 0x7BFF ; stack grows downwards from init but with padding
  ;
  ;Read something from floppy
  ;
  mov [ebr_drive_number],dl
  mov ax,1
  mov cl,1
  mov bx, 0x7E00
  call disk_read
  ;
  ; prints message
  ;
  mov si,msg_loading
  call prints
  cli
  hlt
;
;
;
boot_process_failed:
  mov si,msg_boot_process_failed
  call prints
  jmp wait_key_and_reboot
;
;
;
wait_key_and_reboot:
  mov ah,0
  int 16h
  jmp 0FFFFh:0
;
;
;
.halt:
  cli
  hlt
;
; Disk routines
; convert lba to chs since bootloader only supports chs
; Convert the lba to chs system since that's what a floppy uses
;
lba_to_chs:
  push ax
  push dx
  xor dx,dx   ; dx=0
  div word [bdb_sectors_per_track]
  inc dx
  mov cx,dx
  xor dx,dx ; dx=0
  div word [bdb_heads]
  mov dh,dl
  mov ch,al
  shl ah,6
  or cl,ah
  pop dx
  mov dl,al
  pop ax
  ret
;
; reads info from disk
;
disk_read:
  push ax
  push bx
  push cx
  push dx
  push di
  push cx
  call lba_to_chs
  pop cx
  mov ah,02h
  mov di, 3
;
;
;
.retry:
  pusha
  stc
  int 13h
  jnc .done
  ; reading disk fails
  popa
  call disk_reset
   dec di
  test di,di
  jnz .retry
;
;
;
.fail:
 ; failed
  jmp boot_process_failed
;
;
;
.done:
  popa
  pop di
  pop dx
  pop cx
  pop bx
  pop ax
  ret
;
;
;
disk_reset:
  pusha
  mov ah, 00h
  int 13h
  jc boot_process_failed
  popa
  ret
;
; main program loop ends
;
msg_loading: db "Loading....", ENDL,0
msg_boot_process_failed: db "Reading from disk failed.. Press any key to restart..", ENDL,0

times 510-($-$$) db 0 ; makes the number of bytes =512 since using a floppy to boot

dw 0AA55h ; for bootloader, dw= define words; 2 bytes

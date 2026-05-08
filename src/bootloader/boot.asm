; bootsector defines the starting address as 0x7C00, org is a directive to initiate the starting; legacy booting
org 0x7C00
bits 16 ; emits 16 bits code, directive not instruction

 ;macro defined for a '\n' new line
%define ENDL 0x0D,0x0A
;
; Fat12 header
;
jmp short start
nop

bdb_oem db 'MSWIN4.1'
bdb_bytes_per_sector: dw 512
bdb_sectors_per_cluster: db 1
bdb_reserved_sectors: dw 1
bdb_fat_count: db 2
bdb_dir_entries_count: dw 0E0h
bdb_total_sectors: dw 2880
bdb_media_descriptor_type: db 0F0h
bdb_sectors_per_fat: dw 9
bdb_sectors_per_track: dw 18
bdb_heads: dw 2
bdb_hidden_sectors: dd 0
bdb_large_sector_count: dd 0

; extended boot record
ebr_drive_number: db 0
db 0
ebr_signature: db 29h
ebr_volume_id: db 12h, 45h, 35h, 67h
ebr_volume_label: db ' MY OS      ' ; padded to 11 chars
ebr_system_id: db 'FAT12   ' ; padded to 8 chars


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
  ;
  ;Read something from floppy
  ;
  mov [ebr_drive_number],dl

  mov ax,1
  mov cl,1
  mov bx, 0x7E00
  call disk_read

  ; prints message
  mov si,msg_hello
  call prints

  cli
  hlt

boot_process_failed:
  mov si,msg_boot_process_failed
  call prints
  jmp wait_key_and_reboot

wait_key_and_reboot:
  mov ah,0
  int 16h
  jmp 0FFFFh:0

.halt:
  cli
  hlt

;
; Disk routines
; convert lba to chs since bootloader only supports chs
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

  pop ax
  mov dl,al
  pop ax
  ret
;
; reads from a disk
;
disk_read:
  push ax
  push bx
  push cx
  push dx
  push di

  push cx
  call lba_to_chs
  pop ax

  mov ah,02h
  mov di, 3


.retry:
  pusha
  stc
  int 13h
  jnc .done
  ; read fails
  popa
  call disk_reset

   dec di
  test di,di
  jnz .retry

.fail:
 ; failed
  jmp boot_process_failed

.done:
  popa
  pop di
  pop dx
  pop cx
  pop bx
  pop ax
  ret

disk_reset:
  pusha
  mov ah, 0
  stc
  int 13h
  jc boot_process_failed
  popa
  ret


; main program loop ends

msg_hello: db "Hello World!", ENDL,0
msg_boot_process_failed: db "Reading from disk failed.. Press any key to restart..", ENDL,0

times 510-($-$$) db 0 ; makes the number of bytes =512 since using a floppy to boot

dw 0AA55h ; for bootloader, dw= define word; 2 bytes

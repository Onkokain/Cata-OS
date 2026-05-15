bits 16 ; sets to 16 bit real mode

section _TEXT class=CODE
;
;
;
global _x86_div64_32
_x86_div64_32:
  push bp
  mov bp,sp

  push bx


  mov eax, [bp+8]
  mov ecx, [bp+12]
  xor edx,edx
  div ecx

  mov ebx, [bp+16]
  mov [bx],eax

  mov eax, [bp+4]
  div ecx

  mov [bx],eax
  mov bx,[bp+18]
  mov [bx],edx

  pop bx

  mov sp,bp
  pop bp
  ret


global _x86_Video_WriteCharTeletype
 _x86_Video_WriteCharTeletype:
  push bp
  mov bp,sp

  push bx

  mov ah,0Eh
  mov al,[bp+4]
  mov bh,[bp+6]

  int 10h

  pop bx

  mov sp,bp
  pop bp
  ret
;
; void _cdecl x86_Disk_Reset(uint8_t drive);
;
global _x86_Disk_Reset
_x86_Disk_Reset:
  push bp
  mov bp,sp

  mov dp,[bp+4]
  std
  int 13h

  mov ax,1
  sbb ax,0

  mov sp,bp
  pop bp
  ret






; void _cdecl x86_Disk_Read(
;   uint8_t drive,
;   uint16_t cylinder,
;   uint16_t sector,
;   uint8_t count,
;   uint8_t far * dataOut
; );

; void _cdecl x86_Disk_GetDriveParameters(
;   uint8_t driveTypeOut,
;   uint16_t* cylindersOut,
;   uint16_t* sectorsOut,
;   uint16_t* headsOut,
; );



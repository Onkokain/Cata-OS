%macro EnterRealMode 0
  [bits 32]
  jmp word 18h:.pmode16

  .pmode16:
  [bits 16]

  mov eax,cr0
  and al, ~1
  mov cr0, eax

  jmp word 00h:.rmode

  .rmode:
    mov ax, 0
    mov ds, ax
    mov ss, ax
    sti

%endmacro

%macro EnterProtectedMode 0
  cli

  mov eax, cr0
  or al, 1
  mov cr0, eax

  jmp dword 08h:.pmode

  .pmode:
    [bits 32]

    mov ax, 0x10
    mov ds, ax
    mov ss, ax

%endmacro

;linear addr -> segment:offset addr
%macro LinearToSegOff 4
  ; 1 = linear address
  ; 2 = segment register
  ; 3 = temp 32-bit register
  ; 4 = offset register

  mov %3, %1 ; linear addr -> eax
  shr %3, 4
  mov %2, %4

  mov %3, %1
  and %3, 0xf

%endmacro

global x86_outb

x86_outb:
  [bits 32]
  mov dx, [esp+4]
  mov al, [esp+8]
  out dx, al
  ret

global x86_inb

x86_inb:
  [bits 32]
  mov dx, [esp+4]
  xor eax, eax
  in al, dx
  ret

global RealMode_Putc
RealMode_Putc:
  ; stack frames
  push ebp
  mov ebp, esp

  EnterRealMode
  mov al, [bp+8]
  mov ah, 0xe
  int 10h

  EnterProtectedMode

  mov esp, ebp
  pop ebp
  ret


; void _cdecl x86_Disk_GetDriveParameters(
;   uint8_t driveTypeOut,
;   uint16_t* cylindersOut,
;   uint16_t* sectorsOut,
;   uint16_t* headsOut,
; );

global x86_Disk_GetDriveParameters
x86_Disk_GetDriveParameters:
  [bits 32]

  push ebp
  mov ebp,esp

  EnterRealMode
  [bits 16]

  push es
  push bx
  push esi
  push di


  mov dl,[bp+8]
  mov ah,08h
  mov di,0
  mov es,di
  stc
  int 13h

  mov eax,1
  sbb eax,0


  LinearToSegOff [bp + 12], es, esi, si
  mov es:[si], bl

  mov bl,ch
  mov bh,cl
  shr bh,6
  inc bx

  LinearToSegOff [bp + 16], es, esi, si
  mov es:[si], bx

  xor ch,ch
  and cl,3Fh

  LinearToSegOff [bp+20], es, esi, si
  mov es:[si], cx

  mov cl,dh
  inc cl
  LinearToSegOff [bp+24], es, esi, si
  mov es:[si], cx

  pop di
  pop esi
  pop bx
  pop es

  ; return
  push eax

  EnterProtectedMode
  [bits 32]
  pop eax

  mov esp,ebp
  pop ebp
  ret


global x86_Disk_Reset
x86_Disk_Reset:
  [bits 32]
  push ebp
  mov ebp,esp

  EnterRealMode

  mov ah,0
  mov dl,[bp+8]
  stc
  int 13h

  mov eax,1
  sbb eax,0

  push eax

  EnterProtectedMode
  pop eax

  mov esp,ebp
  pop ebp
  ret


global x86_Disk_Read
x86_Disk_Read:
  [bits 32]
  push ebp
  mov ebp,esp

  EnterRealMode

  push ebx
  push es

  mov dl,[bp + 8]

  mov ch, [bp + 12]
  mov cl, [bp + 13]
  shl cl, 6

  mov al, [bp + 16]
  and al, 3Fh
  or cl, al

  mov dh,[bp + 20]

  mov al, [bp + 24]

  LinearToSegOff [bp + 28], es, ebx, bx

  mov ah,02h
  stc
  int 13h

  mov eax,1
  sbb eax,0

  pop es
  pop ebx

  push eax
  EnterProtectedMode
  pop eax
  mov esp,ebp
  pop ebp
  ret

global x86_Video_GetVbeInfo
x86_Video_GetVbeInfo:
  push ebp
  mov ebp, esp

  EnterRealMode

  LinearToSegOff [bp+8],es,edi,di

  mov eax, 0x4f00
  int 10h

  push eax

  EnterProtectedMode
  pop eax

  mov esp, ebp
  pop ebp
  ret

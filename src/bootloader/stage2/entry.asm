bits 16

section .entry

extern __bss_start
extern __end

;extern _cstart_
global entry

entry:
    cli

    mov [g_BootDrive],dl
    mov ax, ds
    mov ss, ax
    mov sp, 0
    mov bp, sp
    sti

  ; switching to 32 bits protected mode
    call EnableA20
    call LoadGDT

    mov eax,cr0 ; switching to protected mode
    or al,1
    mov cr0,eax
    ; protected mode code segment
    jmp dword 08h:.pmode

.pmode:
  ; 32 bit protected mode starts
  [bits 32]
  ;setting up segments registers
  mov ax,0x10
  mov ds,ax
  mov ss,ax

  ; remove uninit data
  mov edi, __bss_start
  mov ecx, __end
  sub ecx,edi ; size
  mov al,0
  cld
  rep stosb


  xor edx,edx
  mov dl,[g_BootDrive]
  push edx

  cli
  hlt



EnableA20:
  [bits 16]
  call A20WaitInput
  mov al, DisableKeyboard
  out CommandPort,al

  call A20WaitInput
  mov al,ReadCtrlOutputPort
  out CommandPort,al

  call A20WaitOutput
  in al, DataPort
  push eax

  call A20WaitInput
  mov al, WriteCtrlOutputPort
  out CommandPort, al

  call A20WaitInput
  pop eax
  or al,2
  out DataPort,al

  call A20WaitInput
  mov al,EnableKeyboard
  out CommandPort,al

  call A20WaitInput
  ret


A20WaitInput:
  in al, CommandPort
  test al,2
  jnz A20WaitInput
  ret

A20WaitOutput:
  in al, CommandPort
  test al,1
  jnz A20WaitOutput
  ret

LoadGDT: ; global descriptor table
  lgdt [g_GDTDesc]
  ret

DataPort equ 0x60
CommandPort equ 0x64
DisableKeyboard equ 0xAD
EnableKeyboard equ 0xAE
ReadCtrlOutputPort equ 0xD0
WriteCtrlOutputPort equ 0xD1

g_GDT:
        dq 0
;x32 bits code
        dw 0FFFFh
        dw 0
        db 0
        db 10011010b
        db 11001111b
        db 0
;x32 bits data

        dw 0FFFFh
        dw 0
        db 0
        db 10010010b
        db 11001111b
        db 0

;x16 bits code
        dw 0FFFFh
        dw 0
        db 0
        db 10010010b
        db 00001111b
        db 0

;x16 bits data
        dw 0FFFFh
        dw 0
        db 0
        db 10010010b
        db 00001111b
        db 0
    xor dh, dh
    push dx

g_GDTDesc:
  dw g_GDTDesc - g_GDT -1
  dd g_GDT

 g_BootDrive:
  db 0

times 510-($-$$) db 0
dw 0AA55h

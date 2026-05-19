bits 16

section .entry

;extern _cstart_
global entry

entry:
    cli
    mov ax, ds
    mov ss, ax
    mov sp, 0
    mov bp, sp
    sti

  ; switching to 16 bits protected mode
    cli
    call EnableA20
    call loadGDT

    mov eax,cr0
    or al,1
    mov cr0,eax

    jmp dword 08h:.pmode

EnableA20:
  call A20WaitInput
  mov al, KdbControllerDisablekeyboard
  out KdbControllerCommandPort,al

  call A20WaitInput
  mov al,KdbControllerReadCtrlOutputPort
  out KdbControllerCommandPort,al

  call A20WaitOutput
  in al, KdbControllerDataPort
  push eax

  call A20WaitInput
  mov al, KbdControllerWriteCtrlOutputPort
  out KbdControllerCommandPort, al

  call A20WaitInput
  pop eax
  or al,2
  out KbdControllerDataPort,al

  call A20WaitInput
  mov al,KbdControllerEnableKeyboard
  out KbdControllerCommandPort,al

  call A20WaitInput
  ret


A20WaitInput:
  in al, KbdControllerCommandPort
  test al,2
  jnz A20WaitInput
  ret

A20WaitOutput:
  in al, KbdControllerCommandPort
  test al,1
  jnz A20WaitOutput
  ret

LoadGDT:
  

.halt:
  jmp .halt

KbdControllerDataPort equ 0x60
KdbControllerCommandPort equ 0x64
KdbControllerDisableKeyboard equ 0xAD
KdbControllerEnableKeyboard equ 0xAE
KdbControllerReadCtrlOutput equ 0xD0
KdbControllerWriteCtrlOutput equ 0xD1






    xor dh, dh
    push dx
    ;call _cstart_

    cli
    hlt

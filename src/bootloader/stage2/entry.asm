bits 16 ; defines 16 bits code to be used

;defining it as the start of the program
section .entry

;externally defined labels
extern __bss_start
extern __end

extern start

global entry ; defining it as a global label

; main program starts
entry:
    cli ; disabling hardware interrupts

    ;
    mov [g_BootDrive],dl ; moves dl to address 0

    mov ax, ds
    mov ss, ax
    mov sp, 0xFFF0
    mov bp, sp

  ; switching to 32 bits protected mode
    call EnableA20
    call LoadGDT

    mov eax,cr0
    or al , 1
    mov cr0,eax

    ; far jump into protected mode
    jmp dword 08h:.pmode

.pmode:
  [bits 32]
  ; 32 bit protected mode starts
  ;setting up segments registers
  mov ax,0x10
  mov ds,ax
  mov ss,ax
  mov es,ax

  ; remove uninit data ;C equivalent code: memset(__bss_start, 0, __end - __bss_start);

  mov edi, __bss_start ; address of start
  mov ecx, __end ; address of end
  sub ecx,edi ; size=(end-start)
  mov al,0
  cld ; clears directory flag
  rep stosb ; repeat store string byte; stores at edi and repeats it ecx times

  ; saving boot drive since it gets cleared during runtime
  xor edx,edx ; setting edx=0
  mov dl,[g_BootDrive]
  push edx
  call start

  cli ; disabling hardware enterrupts
  hlt ; end


; normally 20 address busses but a20 is defined as the 21st such that it can transfer from A19 to A01 without any error; expected behavior
EnableA20: ; enable 20th data segment so that memories above 1mb is accessible
  [bits 16] ; 16 bits real mode code
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
  [bits 16] ; 16 bits real mode code
  in al, CommandPort
  test al,2 ; bitwise and operator
  jnz A20WaitInput ; jump if not zero
  ret; returns

A20WaitOutput:
  [bits 16] ; 16 bits real mode code
  in al, CommandPort ; read one byte from commandport and store in al
  test al,1 ; bitwise and
  jz A20WaitOutput ; jump if not 0
  ret

LoadGDT: ; global descriptor
  [bits 16] ; 16 bits real mode code
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

g_GDT_end:

g_GDTDesc:
  dw g_GDT_end - g_GDT -1
  dd g_GDT

 g_BootDrive:
  db 0


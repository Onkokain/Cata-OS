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



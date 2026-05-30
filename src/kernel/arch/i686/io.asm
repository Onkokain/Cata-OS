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

global i686_panic
i686_panic:
  cli
  hlt

;uint8_t __attribute__((cdecl)) x86_sti();
global x86_EnableInterrupts
x86_EnableInterrupts:
  sti
  ret

;uint8_t __attribute__((cdecl)) x86_cli();
global x86_DisableInterrupts
x86_DisableInterrupts:
  cli
  ret


global crash_me
crash_me:
  ret

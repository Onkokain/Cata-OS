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

    cli
    call EnableA20
    call loadGDT

    mov eax,cr0
    or al,1
    mov cr0,eax

    jmp dword 08h:.pmode

.rmode:
  mov ax,0
  mov ds,ax
  mov ss,ax

  sti

  mov si,g_HelloR


    xor dh, dh
    push dx
    ;call _cstart_

    cli
    hlt

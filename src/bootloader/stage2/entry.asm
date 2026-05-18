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

    xor dh, dh
    push dx
    ;call _cstart_

    cli
    hlt

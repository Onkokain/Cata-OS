org 0x0000
bits 16


%define ENDL 0x0D, 0x0A ; \n new character line


start:
    ; print hello world message
    mov si, msg_hello
    call prints

.halt:
    cli
    hlt
    jmp .halt

;
; Prints a string to the screen
;
; same as that in sector1
prints:
    ; save registers that will be modified
    push si
    push ax
    push bx

.loop:
    lodsb
    or al, al
    jz .done

    mov ah, 0x0E        ; calls the bios interrupt
    mov bh, 0           ; prints the message on page 0
    int 0x10

    jmp .loop

.done:
    pop bx
    pop ax
    pop si
    ret

msg_hello: db 'Hello world!', ENDL, 0

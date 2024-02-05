section .data
    newline db 0xA

section .bss
    num resb 10

section .text
global _start

_start:
    mov eax, 1234
    mov ebx, 10
    mov ecx, num
    add ecx, 9

convert_loop:
    xor edx, edx
    div ebx
    add dl, '0'
    dec ecx
    mov [ecx], dl
    test eax, eax
    jnz convert_loop

    mov eax, 4
    mov ebx, 1
    lea ecx, [ecx]
    mov edx, 10
    sub edx, ecx
    sub edx, num
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    mov eax, 1
    xor ebx, ebx
    int 0x80


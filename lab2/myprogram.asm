; Author: Xiyao	Xu
; Date: January 29, 202
section .data
    newline db 0xA                ; Define a newline character (ASCII code 10)

section .bss
    num resb 10                   ; Reserve 10 bytes in the uninitialized data section for the number string

section .text
global _start

_start:
    mov eax, 1234                 ; Load the number 1234 into the EAX register
    mov ebx, 10                   ; Set EBX to 10, to use as the divisor for decimal conversion
    mov ecx, num                  ; Load the address of 'num' into ECX
    add ecx, 9                    ; Move ECX to point to the end of the 'num' buffer

convert_loop:
    xor edx, edx                  ; Clear EDX register for DIV operation
    div ebx                       ; Divide EAX by EBX (10), result in EAX, remainder in EDX
    add dl, '0'                   ; Convert the remainder to an ASCII character
    dec ecx                       ; Move back one byte in the buffer
    mov [ecx], dl                 ; Store the ASCII character in the buffer
    test eax, eax                 ; Test if the quotient in EAX is zero
    jnz convert_loop              ; If not zero, continue the loop

    mov eax, 4                    ; System call number for sys_write
    mov ebx, 1                    ; File descriptor 1 is standard output
    lea ecx, [ecx]                ; Load effective address of the start of the number string into ECX
    mov edx, 10                   ; Number of bytes to write (size of buffer)
    sub edx, ecx                  ; Adjust EDX to write only the converted number
    sub edx, num
    int 0x80                      ; Call kernel

    mov eax, 4                    ; System call number for sys_write
    mov ebx, 1                    ; File descriptor 1 is standard output
    mov ecx, newline              ; Address of the newline character
    mov edx, 1                    ; Number of bytes to write (1 byte for newline)
    int 0x80                      ; Call kernel

    mov eax, 1                    ; System call number for sys_exit
    xor ebx, ebx                  ; Return a status of 0
    int 0x80                      ; Call kernel


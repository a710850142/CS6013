section .data
helloString db "hello", 0

section .text
global sayHello
global myPuts
extern puts
extern write
extern gettimeofday

sayHello:
    mov rdi, helloString
    call puts
    ret

myPuts:
    mov rax, 1
    mov rdi, 1
    mov rdx, rsi
    syscall
    ret

section .bss
timeval resb 16

myGTOD:
    sub rsp, 16
    mov rdi, rsp
    xor rsi, rsi
    mov rax, 96
    syscall
    mov rax, [rsp]
    mov rdx, [rsp + 8]
    add rsp, 16
    ret


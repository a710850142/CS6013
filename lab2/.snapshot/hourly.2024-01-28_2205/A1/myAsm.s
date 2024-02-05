section .data
helloString: db "Hello, World!", 0

section .bss
resb 8

section .text
global sayHello
global myPuts
global myGTOD
extern puts
extern write
extern gettimeofday

sayHello:
    mov rdi, helloString
    call puts
    ret

myPuts:
    mov rax, 1
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 1
    syscall
    ret

myGTOD:
    sub rsp, 16
    mov rdi, rsp
    xor esi, esi
    call gettimeofday
    mov rax, [rsp]
    mov rdx, [rsp+8]
    add rsp, 16
    ret


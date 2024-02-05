section .data
helloString db "hello", 0

section .text
global sayHello
global myPuts
extern puts
extern write

sayHello:
    mov rdi, helloString
    call puts
    ret

myPuts:
    mov rax, 1          
    mov rdi, 1          
    mov rsi, [rsp + 8]  
    mov rdx, [rsp + 16] 
    syscall             
    ret


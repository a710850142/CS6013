#!/bin/bash

g++ -c main.cpp -o main.o
nasm -f elf64 myAsm.s -o myAsm.o
g++ -o myProgram main.o myAsm.o

output=$(./myProgram)

echo "Checking output..."

if echo "$output" | grep -q "Hello, World!"; then
    echo "sayHello function output is correct"
else
    echo "sayHello function output is incorrect"
    exit 1
fi

if echo "$output" | grep -q "Hello from myPuts"; then
    echo "myPuts function output is correct"
else
    echo "myPuts function output is incorrect"
    exit 1
fi

if echo "$output" | grep -q "Seconds: .* Microseconds: .*"; then
    echo "myGTOD function output format is correct"
else
    echo "myGTOD function output format is incorrect"
    exit 1
fi

echo "Test completed."


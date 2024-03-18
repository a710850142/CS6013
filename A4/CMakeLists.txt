cmake_minimum_required(VERSION 3.27)
project(A4)

set(CMAKE_CXX_STANDARD 17)

add_executable(A4 main.cpp
        HashTable.cpp
        MyMalloc.cpp
        HashTable.h
        MyMalloc.h)

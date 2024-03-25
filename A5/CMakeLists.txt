cmake_minimum_required(VERSION 3.27)
project(A5)

set(CMAKE_CXX_STANDARD 17)

add_executable(A5
        SerialQueue.hpp
        SerialQueueTests.cpp
        ConcurrentQueue.hpp
        ConcurrentQueueTest.cpp)

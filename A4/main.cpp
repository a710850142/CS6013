#include "MyMalloc.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>

void* my_malloc(size_t size) {
    static MyMalloc myMalloc;
    return myMalloc.allocate(size);
}

void my_free(void* ptr) {
    static MyMalloc myMalloc;
    myMalloc.deallocate(ptr);
}

int main() {
    // 初始化自定义内存管理器
    MyMalloc myMalloc;

    // 性能测试：自定义内存分配与释放
    auto start_custom = std::chrono::high_resolution_clock::now();

    std::vector<void*> custom_allocations;
    const size_t smallSize = 128;
    const size_t largeSize = 1024 * 1024; // 1MB

    // 分配小内存块并使用
    for (int i = 0; i < 1000; ++i) {
        void* ptr = my_malloc(smallSize);
        memset(ptr, i % 256, smallSize);
        custom_allocations.push_back(ptr);
    }

    // 分配大内存块并使用
    for (int i = 0; i < 10; ++i) {
        void* ptr = my_malloc(largeSize);
        memset(ptr, i % 256, largeSize);
        custom_allocations.push_back(ptr);
    }

    // 释放内存
    for (void* ptr : custom_allocations) {
        my_free(ptr);
    }

    auto end_custom = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_custom = end_custom - start_custom;
    std::cout << "Custom allocation/deallocation time: " << diff_custom.count() << " s\n";

    // 性能测试：系统内存分配与释放
    auto start_system = std::chrono::high_resolution_clock::now();

    std::vector<void*> system_allocations;

    // 系统分配小内存块并使用
    for (int i = 0; i < 1000; ++i) {
        void* ptr = malloc(smallSize);
        memset(ptr, i % 256, smallSize);
        system_allocations.push_back(ptr);
    }

    // 系统分配大内存块并使用
    for (int i = 0; i < 10; ++i) {
        void* ptr = malloc(largeSize);
        memset(ptr, i % 256, largeSize);
        system_allocations.push_back(ptr);
    }

    // 系统释放内存
    for (void* ptr : system_allocations) {
        free(ptr);
    }

    auto end_system = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_system = end_system - start_system;
    std::cout << "System allocation/deallocation time: " << diff_system.count() << " s\n";

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

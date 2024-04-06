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
    // Initialize a custom memory manager
    MyMalloc myMalloc;

    // Performance test: Custom memory allocation and release
    auto start_custom = std::chrono::high_resolution_clock::now();

    std::vector<void*> custom_allocations;
    const size_t smallSize = 128;
    const size_t largeSize = 1024 * 1024; // 1MB

    // Allocate small memory block and use
    for (int i = 0; i < 1000; ++i) {
        void* ptr = my_malloc(smallSize);
        memset(ptr, i % 256, smallSize);
        custom_allocations.push_back(ptr);
    }

    // Allocate large memory block and use
    for (int i = 0; i < 10; ++i) {
        void* ptr = my_malloc(largeSize);
        memset(ptr, i % 256, largeSize);
        custom_allocations.push_back(ptr);
    }

    // free memory
    for (void* ptr : custom_allocations) {
        my_free(ptr);
    }

    auto end_custom = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_custom = end_custom - start_custom;
    std::cout << "Custom allocation/deallocation time: " << diff_custom.count() << " s\n";

    // Performance test: system memory allocation and release
    auto start_system = std::chrono::high_resolution_clock::now();

    std::vector<void*> system_allocations;

    // The system allocates small memory blocks and uses
    for (int i = 0; i < 1000; ++i) {
        void* ptr = malloc(smallSize);
        memset(ptr, i % 256, smallSize);
        system_allocations.push_back(ptr);
    }

    // The system allocates a large memory block and uses
    for (int i = 0; i < 10; ++i) {
        void* ptr = malloc(largeSize);
        memset(ptr, i % 256, largeSize);
        system_allocations.push_back(ptr);
    }

    // System releases memory
    for (void* ptr : system_allocations) {
        free(ptr);
    }

    auto end_system = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_system = end_system - start_system;
    std::cout << "System allocation/deallocation time: " << diff_system.count() << " s\n";

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

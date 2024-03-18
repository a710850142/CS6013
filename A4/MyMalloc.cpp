// MyMalloc.cpp
#include "MyMalloc.h"
#include <unistd.h>
#include <sys/mman.h>

MyMalloc::MyMalloc() : pageSize(getpagesize()) {}

size_t MyMalloc::alignToPageSize(size_t size) {
    return (size + pageSize - 1) & (~(pageSize - 1));
}

void* MyMalloc::allocate(size_t bytesToAllocate) {
    size_t size = alignToPageSize(bytesToAllocate);
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        return nullptr;
    }
    hashTable.insert(ptr, size);
    return ptr;
}

void MyMalloc::deallocate(void* ptr) {
    size_t size = hashTable.find(ptr);
    if (size > 0) {
        munmap(ptr, size);
        hashTable.remove(ptr);
    }
}

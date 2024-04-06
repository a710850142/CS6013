#include "MyMalloc.h"
#include <unistd.h> // Includes POSIX operating system API for system calls like getpagesize.
#include <sys/mman.h> // Includes memory management declarations for mmap and munmap.

// Constructor of the MyMalloc class. Initializes pageSize with the system's memory page size.
MyMalloc::MyMalloc() : pageSize(getpagesize()) {}

// Private method that aligns a given size to the nearest page size boundary.
size_t MyMalloc::alignToPageSize(size_t size) {
    // Adjusts the size to be a multiple of pageSize.
    return (size + pageSize - 1) & (~(pageSize - 1));
}

// Allocates a memory block of specified size, aligns it to page size, and returns a pointer to it.
void* MyMalloc::allocate(size_t bytesToAllocate) {
    size_t size = alignToPageSize(bytesToAllocate); // Aligns the requested size to page size.
    // Attempts to allocate memory using mmap, setting appropriate protection flags and options.
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) { // Checks if mmap call failed.
        return nullptr; // Returns nullptr if memory allocation failed.
    }
    hashTable.insert(ptr, size); // Records the allocated memory in the hash table.
    return ptr; // Returns the pointer to the allocated memory.
}

// Deallocates a previously allocated memory block pointed to by ptr.
void MyMalloc::deallocate(void* ptr) {
    size_t size = hashTable.find(ptr); // Retrieves the size of the memory block to be deallocated.
    if (size > 0) { // Checks if the memory block exists.
        munmap(ptr, size); // Frees the memory block using munmap.
        hashTable.remove(ptr); // Removes the entry for the memory block from the hash table.
    }
}

// MyMalloc.hpp
#ifndef MYMALLOC_HPP // Prevents the header file from being included multiple times in the same file.
#define MYMALLOC_HPP

#include "HashTable.h" // Includes the HashTable header file for managing hash table operations.

// Defines the MyMalloc class that provides custom memory allocation and deallocation functionalities.
class MyMalloc {
private:
    LinearProbingHashTable hashTable; // A hash table to keep track of allocated memory blocks using linear probing.
    size_t pageSize; // The size of the memory page. Used for aligning allocated memory blocks.

    // Private method that aligns a given size to the nearest page size boundary.
    size_t alignToPageSize(size_t size);

public:
    MyMalloc(); // Constructor of the MyMalloc class. Initializes internal data structures or variables.

    // Allocates a memory block of the specified size and returns a pointer to the beginning of the block.
    void* allocate(size_t bytesToAllocate);

    // Deallocates a previously allocated memory block pointed to by ptr.
    void deallocate(void* ptr);
};

#endif // MYMALLOC_HPP

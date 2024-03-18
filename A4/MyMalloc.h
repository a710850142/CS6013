// MyMalloc.hpp
#ifndef MYMALLOC_HPP
#define MYMALLOC_HPP

#include "HashTable.h"

class MyMalloc {
private:
    LinearProbingHashTable hashTable;
    size_t pageSize;

    size_t alignToPageSize(size_t size);

public:
    MyMalloc();
    void* allocate(size_t bytesToAllocate);
    void deallocate(void* ptr);
};

#endif

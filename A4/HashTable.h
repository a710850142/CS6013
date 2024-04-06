#ifndef LINEAR_PROBING_HASH_TABLE_HPP
#define LINEAR_PROBING_HASH_TABLE_HPP

#include <cstddef>
#include <sys/mman.h>

// Define the hash table entry structure
struct HashEntry {
    void* key;   // storage address
    size_t size; // allocated size
    bool active; // Is it occupied?

    HashEntry();
};

class LinearProbingHashTable {
private:
    HashEntry* table; // Hash table array
    size_t capacity;  // Hash table capacity
    size_t elementCount; // The current number of elements in the hash table
    const float maxLoadFactor; // maximum load factor

    void growAndRehash();
    void insertHelper(void* key, size_t size);

public:
    LinearProbingHashTable(size_t initCapacity = 1024);
    ~LinearProbingHashTable();

    void insert(void* key, size_t size);
    size_t find(void* key);
    void remove(void* key);

    size_t hash(void *ptr);
};

#endif

#include "HashTable.h"
#include <cassert>
#include <cstring>

// HashEntry constructor implementation
HashEntry::HashEntry() : key(nullptr), size(0), active(false) {}

// LinearProbingHashTable constructor implementation
LinearProbingHashTable::LinearProbingHashTable(size_t initCapacity)
        : capacity(initCapacity), elementCount(0), maxLoadFactor(0.75) {
    table = (HashEntry*)mmap(nullptr, capacity * sizeof(HashEntry),
                             PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(table != MAP_FAILED); // Make sure the mmap call is successful
    memset(table, 0, capacity * sizeof(HashEntry)); // initialization
}

// LinearProbingHashTable destructor implementation
LinearProbingHashTable::~LinearProbingHashTable() {
    munmap(table, capacity * sizeof(HashEntry)); // free memory
}

// Calculate hash value
size_t LinearProbingHashTable::hash(void* ptr) {
    return reinterpret_cast<size_t>(ptr) >> 3;
}

// Expand and rehash
void LinearProbingHashTable::growAndRehash() {
    size_t oldCapacity = capacity;
    capacity *= 2; // Double capacity

    HashEntry* oldTable = table;
    table = (HashEntry*)mmap(nullptr, capacity * sizeof(HashEntry),
                             PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(table != MAP_FAILED); // Make sure the mmap call is successful

    memset(table, 0, capacity * sizeof(HashEntry)); // Initialize new table

    size_t i;
    for (i = 0; i < oldCapacity; i++) {
        if (oldTable[i].active) {
            insertHelper(oldTable[i].key, oldTable[i].size); // Reinsert into new table
        }
    }

    munmap(oldTable, oldCapacity * sizeof(HashEntry)); // Release old table memory
}

// Insert auxiliary function for re-insertion after expansion
void LinearProbingHashTable::insertHelper(void* key, size_t size) {
    size_t index = hash(key) % capacity;
    while (table[index].active) {
        if (table[index].key == key) {
            // If the same key is found, update its size
            table[index].size = size;
            return;
        }
        index = (index + 1) % capacity; // Linear detection
    }
    table[index].key = key;
    table[index].size = size;
    table[index].active = true;
}

// insert
void LinearProbingHashTable::insert(void* key, size_t size) {
    if ((elementCount + 1) > capacity * maxLoadFactor) {
        growAndRehash();
    }
    insertHelper(key, size);
    elementCount++;
}

// Find
size_t LinearProbingHashTable::find(void* key) {
    size_t index = hash(key) % capacity;
    while (table[index].active) {
        if (table[index].key == key) {
            return table[index].size; // Return the size found
        }
        index = (index + 1) % capacity; // If the current slot is already occupied, try the next slot
        if (!table[index].active) {
            break; // If the next slot is not occupied, stop searching
        }
    }
    return 0; // not found
}

// delete
void LinearProbingHashTable::remove(void* key) {
    size_t index = hash(key) % capacity;
    while (table[index].active) {
        if (table[index].key == key) {
            table[index].active = false; // Mark as inactive
            elementCount--;
            // Note: The memory release operation is not performed here, it is just marked as inactive in the hash table.
            return;
        }
        index = (index + 1) % capacity; // Linearly probe the next slot
    }
}

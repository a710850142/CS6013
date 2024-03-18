#ifndef LINEAR_PROBING_HASH_TABLE_HPP
#define LINEAR_PROBING_HASH_TABLE_HPP

#include <cstddef> // size_t的定义
#include <sys/mman.h> // mmap和munmap的定义

// 定义哈希表条目结构
struct HashEntry {
    void* key;   // 存储地址
    size_t size; // 分配的大小
    bool active; // 是否被占用

    HashEntry();
};

class LinearProbingHashTable {
private:
    HashEntry* table; // 哈希表数组
    size_t capacity;  // 哈希表容量
    size_t elementCount; // 哈希表当前元素数量
    const float maxLoadFactor; // 最大装载因子

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

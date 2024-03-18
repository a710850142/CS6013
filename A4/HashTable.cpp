#include "HashTable.h"
#include <cassert>
#include <cstring>

// HashEntry构造函数实现
HashEntry::HashEntry() : key(nullptr), size(0), active(false) {}

// LinearProbingHashTable构造函数实现
LinearProbingHashTable::LinearProbingHashTable(size_t initCapacity)
        : capacity(initCapacity), elementCount(0), maxLoadFactor(0.75) {
    table = (HashEntry*)mmap(nullptr, capacity * sizeof(HashEntry),
                             PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(table != MAP_FAILED); // 确保mmap调用成功
    memset(table, 0, capacity * sizeof(HashEntry)); // 初始化
}

// LinearProbingHashTable析构函数实现
LinearProbingHashTable::~LinearProbingHashTable() {
    munmap(table, capacity * sizeof(HashEntry)); // 释放内存
}

// 计算哈希值
size_t LinearProbingHashTable::hash(void* ptr) {
    return reinterpret_cast<size_t>(ptr) >> 3;
}

// 扩容并重新哈希
void LinearProbingHashTable::growAndRehash() {
    size_t oldCapacity = capacity;
    capacity *= 2; // 容量加倍

    HashEntry* oldTable = table;
    table = (HashEntry*)mmap(nullptr, capacity * sizeof(HashEntry),
                             PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(table != MAP_FAILED); // 确保mmap调用成功

    memset(table, 0, capacity * sizeof(HashEntry)); // 初始化新表

    size_t i;
    for (i = 0; i < oldCapacity; i++) {
        if (oldTable[i].active) {
            insertHelper(oldTable[i].key, oldTable[i].size); // 重新插入到新表
        }
    }

    munmap(oldTable, oldCapacity * sizeof(HashEntry)); // 释放旧表内存
}

// 插入辅助函数，用于扩容后重新插入
void LinearProbingHashTable::insertHelper(void* key, size_t size) {
    size_t index = hash(key) % capacity;
    while (table[index].active) {
        if (table[index].key == key) {
            // 如果找到相同的键，更新其大小
            table[index].size = size;
            return;
        }
        index = (index + 1) % capacity; // 线性探测
    }
    table[index].key = key;
    table[index].size = size;
    table[index].active = true;
}

// 插入
void LinearProbingHashTable::insert(void* key, size_t size) {
    if ((elementCount + 1) > capacity * maxLoadFactor) {
        growAndRehash();
    }
    insertHelper(key, size);
    elementCount++;
}

// 查找
size_t LinearProbingHashTable::find(void* key) {
    size_t index = hash(key) % capacity;
    while (table[index].active) {
        if (table[index].key == key) {
            return table[index].size; // 返回找到的大小
        }
        index = (index + 1) % capacity; // 如果当前槽已被占用，则尝试下一个槽
        if (!table[index].active) {
            break; // 如果下一个槽未被占用，停止查找
        }
    }
    return 0; // 未找到
}

// 删除
void LinearProbingHashTable::remove(void* key) {
    size_t index = hash(key) % capacity;
    while (table[index].active) {
        if (table[index].key == key) {
            table[index].active = false; // 标记为不活跃
            elementCount--;
            // 注意：这里不执行内存释放操作，只是在哈希表中标记为不活跃
            return;
        }
        index = (index + 1) % capacity; // 线性探测下一个槽
    }
}

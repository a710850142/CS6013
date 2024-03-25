#include "SerialQueue.hpp"
#include <iostream>

int main() {
    // 测试静态分配的队列
    std::cout << "Testing static allocation...\n";
    SerialQueue<int> queue;

    std::cout << "Enqueue 1, 2, 3\n";
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    int value;
    while (queue.dequeue(&value)) {
        std::cout << "Dequeued: " << value << std::endl;
    }

    // 测试动态分配的队列
    std::cout << "\nTesting dynamic allocation...\n";
    SerialQueue<int>* dynamicQueue = new SerialQueue<int>();

    std::cout << "Enqueue 4, 5, 6\n";
    dynamicQueue->enqueue(4);
    dynamicQueue->enqueue(5);
    dynamicQueue->enqueue(6);

    while (dynamicQueue->dequeue(&value)) {
        std::cout << "Dequeued from dynamic queue: " << value << std::endl;
    }

    delete dynamicQueue; // 清理动态分配的队列

    return 0;
}

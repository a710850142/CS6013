#include <iostream>
#include <vector>
#include <thread>
#include "ConcurrentQueue.hpp"

void producer(ConcurrentQueue<int>& queue, int num_ints) {
    for (int i = 0; i < num_ints; ++i) {
        queue.enqueue(i);
    }
}

void consumer(ConcurrentQueue<int>& queue, int num_ints) {
    int temp;
    for (int i = 0; i < num_ints; ++i) {
        while (!queue.dequeue(&temp)) {
            // 繁忙等待（Busy Wait），实际应用中应避免这种方式
            std::this_thread::yield(); // 让出CPU时间片，避免完全占用一个核
        }
    }
}

bool testQueue(int num_producers, int num_consumers, int num_ints) {
    ConcurrentQueue<int> queue;
    std::vector<std::thread> threads;

    // 创建生产者线程
    for (int i = 0; i < num_producers; ++i) {
        threads.emplace_back(producer, std::ref(queue), num_ints);
    }

    // 创建消费者线程
    for (int i = 0; i < num_consumers; ++i) {
        threads.emplace_back(consumer, std::ref(queue), num_ints);
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    // 验证队列的最终状态
    int expected_size = num_producers * num_ints - num_consumers * num_ints;
    return queue.size() == expected_size;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " num_producers num_consumers num_ints\n";
        return 1;
    }

    int num_producers = std::stoi(argv[1]);
    int num_consumers = std::stoi(argv[2]);
    int num_ints = std::stoi(argv[3]);

    if (testQueue(num_producers, num_consumers, num_ints)) {
        std::cout << "Test passed.\n";
    } else {
        std::cout << "Test failed.\n";
    }

    return 0;
}

#include "SerialQueue.hpp"
#include <iostream>

int main() {
    // Testing queue with static allocation
    std::cout << "Testing static allocation...\n";
    SerialQueue<int> queue; // Create a queue with static allocation

    // Enqueue elements 1, 2, and 3 into the queue
    std::cout << "Enqueue 1, 2, 3\n";
    queue.enqueue(1); // Add 1 to the queue
    queue.enqueue(2); // Add 2 to the queue
    queue.enqueue(3); // Add 3 to the queue

    int value; // Variable to store dequeued values
    // Dequeue and print elements until the queue is empty
    while (queue.dequeue(&value)) {
        std::cout << "Dequeued: " << value << std::endl; // Print the dequeued value
    }

    // Testing queue with dynamic allocation
    std::cout << "\nTesting dynamic allocation...\n";
    SerialQueue<int>* dynamicQueue = new SerialQueue<int>(); // Dynamically allocate a new queue

    // Enqueue elements 4, 5, and 6 into the dynamic queue
    std::cout << "Enqueue 4, 5, 6\n";
    dynamicQueue->enqueue(4); // Add 4 to the queue
    dynamicQueue->enqueue(5); // Add 5 to the queue
    dynamicQueue->enqueue(6); // Add 6 to the queue

    // Dequeue and print elements from the dynamic queue until it's empty
    while (dynamicQueue->dequeue(&value)) {
        std::cout << "Dequeued from dynamic queue: " << value << std::endl; // Print the dequeued value from dynamic queue
    }

    delete dynamicQueue; // Clean up the dynamically allocated queue

    return 0;
}

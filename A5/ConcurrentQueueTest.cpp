#include <iostream>
#include <vector>
#include <thread>
#include "ConcurrentQueue.hpp" // Include the header for the ConcurrentQueue template

// Producer function that adds integers to the queue
void producer(ConcurrentQueue<int>& queue, int num_ints) {
    for (int i = 0; i < num_ints; ++i) {
        queue.enqueue(i); // Enqueue integers from 0 to num_ints-1 into the queue
    }
}

// Consumer function that removes integers from the queue
void consumer(ConcurrentQueue<int>& queue, int num_ints) {
    int temp;
    for (int i = 0; i < num_ints; ++i) {
        while (!queue.dequeue(&temp)) { // Attempt to dequeue an element
            // Busy wait: In actual applications, this approach should be avoided
            std::this_thread::yield(); // Yield CPU time slice to avoid fully occupying a core
        }
    }
}

// Test function to validate the concurrent queue with multiple producers and consumers
bool testQueue(int num_producers, int num_consumers, int num_ints) {
    ConcurrentQueue<int> queue; // Instantiate the queue
    std::vector<std::thread> threads; // Vector to hold all producer and consumer threads

    // Create producer threads
    for (int i = 0; i < num_producers; ++i) {
        threads.emplace_back(producer, std::ref(queue), num_ints); // Add producer threads
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; ++i) {
        threads.emplace_back(consumer, std::ref(queue), num_ints); // Add consumer threads
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join(); // Join each thread with the main thread
    }

    // Validate the final state of the queue
    int expected_size = num_producers * num_ints - num_consumers * num_ints; // Calculate expected size
    return queue.size() == expected_size; // Return true if the actual size matches the expected size
}

// Main function
int main(int argc, char** argv) {
    if (argc != 4) { // Check for correct number of command line arguments
        std::cerr << "Usage: " << argv[0] << " num_producers num_consumers num_ints\n"; // Print usage message
        return 1; // Exit with error code
    }

    int num_producers = std::stoi(argv[1]); // Convert first argument to integer
    int num_consumers = std::stoi(argv[2]); // Convert second argument to integer
    int num_ints = std::stoi(argv[3]); // Convert third argument to integer

    // Run the test and print the result
    if (testQueue(num_producers, num_consumers, num_ints)) {
        std::cout << "Test passed.\n";
    } else {
        std::cout << "Test failed.\n";
    }

    return 0; // Normal program termination
}

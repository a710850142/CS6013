#pragma once

#include <mutex>

template <typename T>
class ConcurrentQueue {
public:
    // Constructor initializes an empty queue
    ConcurrentQueue() : head_(nullptr), tail_(nullptr), size_(0) {}

    // Destructor clears the queue to prevent memory leaks
    ~ConcurrentQueue() {
        clear();
    }

    // Adds a new element at the end of the queue
    void enqueue(const T& x) {
        std::unique_lock<std::mutex> lock(mutex_); // Locks the queue for safe concurrent access
        Node* newNode = new Node{x, nullptr}; // Creates a new node
        if (tail_ == nullptr) { // If the queue is empty, new node becomes both head and tail
            head_ = tail_ = newNode;
        } else { // Otherwise, it's added to the end and becomes the new tail
            tail_->next = newNode;
            tail_ = newNode;
        }
        ++size_; // Increment the size of the queue
    }

    // Removes and returns the element at the front of the queue
    bool dequeue(T* ret) {
        std::unique_lock<std::mutex> lock(mutex_); // Locks the queue for safe concurrent access
        if (head_ == nullptr) { // If the queue is empty, return false
            return false;
        }
        Node* temp = head_; // Temporary pointer to the head
        *ret = head_->data; // Sets the output parameter to the head's data
        head_ = head_->next; // Moves the head pointer to the next node
        if (head_ == nullptr) { // If the queue becomes empty, reset tail to nullptr
            tail_ = nullptr;
        }
        delete temp; // Frees the memory of the removed head
        --size_; // Decrement the size of the queue
        return true;
    }

    // Returns the number of elements in the queue
    int size() const {
        std::unique_lock<std::mutex> lock(mutex_); // Locks the queue for safe concurrent access
        return size_;
    }

private:
    struct Node {
        T data; // Data stored in the node
        Node* next; // Pointer to the next node in the queue
        Node(const T& data, Node* next = nullptr) : data(data), next(next) {} // Constructor for node
    };

    Node* head_; // Pointer to the front of the queue
    Node* tail_; // Pointer to the end of the queue
    mutable std::mutex mutex_; // Mutex to protect access to head_, tail_, and size_
    int size_; // Number of elements in the queue

    // Utility function to clear the queue
    void clear() {
        while (head_ != nullptr) {
            Node* temp = head_->next;
            delete head_; // Deletes the head node
            head_ = temp; // Moves head to the next node
        }
        tail_ = nullptr; // Resets tail to nullptr
        size_ = 0; // Resets size to 0
    }
};

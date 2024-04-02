#pragma once

template <typename T>
class SerialQueue {
public:
    // Constructor initializes an empty queue
    SerialQueue() : head_(nullptr), tail_(nullptr), size_(0) {}

    // Destructor deallocates all the nodes in the queue
    ~SerialQueue() {
        while (head_ != nullptr) {
            Node* temp = head_->next; // Save next node
            delete head_; // Delete the current node
            head_ = temp; // Move head to next node
        }
    }

    // Enqueue a new element at the end of the queue
    void enqueue(const T& x) {
        Node* newNode = new Node{x, nullptr}; // Create a new node
        if (tail_ == nullptr) {
            // If the queue is empty, the new node is both head and tail
            head_ = tail_ = newNode;
        } else {
            // If the queue is not empty, add the new node to the end and update tail
            tail_->next = newNode;
            tail_ = newNode;
        }
        ++size_; // Increment the queue size
    }

    // Dequeue an element from the front of the queue. Returns false if the queue is empty
    bool dequeue(T* ret) {
        if (head_ == nullptr) {
            return false; // Return false if the queue is empty
        }
        Node* temp = head_; // Temporary node to hold the head
        *ret = head_->data; // Set the return value to the data of the head node
        head_ = head_->next; // Move the head to the next node
        if (head_ == nullptr) {
            // If the queue is now empty, set tail to nullptr as well
            tail_ = nullptr;
        }
        delete temp; // Free the memory of the dequeued node
        --size_; // Decrement the queue size
        return true;
    }

    // Returns the number of elements in the queue
    int size() const { return size_; }

private:
    struct Node {
        T data; // The data stored in the node
        Node* next; // Pointer to the next node in the queue
        Node(const T& data, Node* next) : data(data), next(next) {}
    };

    Node* head_; // Pointer to the head of the queue
    Node* tail_; // Pointer to the tail of the queue
    int size_; // The number of elements in the queue
};

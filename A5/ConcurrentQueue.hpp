#pragma once

#include <mutex>

template <typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue() : head_(nullptr), tail_(nullptr), size_(0) {}

    ~ConcurrentQueue() {
        clear();
    }

    void enqueue(const T& x) {
        std::unique_lock<std::mutex> lock(mutex_);
        Node* newNode = new Node{x, nullptr};
        if (tail_ == nullptr) {
            head_ = tail_ = newNode;
        } else {
            tail_->next = newNode;
            tail_ = newNode;
        }
        ++size_;
    }

    bool dequeue(T* ret) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (head_ == nullptr) {
            return false;
        }
        Node* temp = head_;
        *ret = head_->data;
        head_ = head_->next;
        if (head_ == nullptr) {
            tail_ = nullptr;
        }
        delete temp;
        --size_;
        return true;
    }

    int size() const {
        std::unique_lock<std::mutex> lock(mutex_); const;
        return size_;
    }

private:
    struct Node {
        T data;
        Node* next;
        Node(const T& data, Node* next = nullptr) : data(data), next(next) {}
    };

    Node* head_;
    Node* tail_;
    mutable std::mutex mutex_; // Protects access to head_, tail_, and size_
    int size_;

    // Utility function to clear the queue
    void clear() {
        while (head_ != nullptr) {
            Node* temp = head_->next;
            delete head_;
            head_ = temp;
        }
        tail_ = nullptr;
        size_ = 0;
    }
};

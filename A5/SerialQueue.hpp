#pragma once

template <typename T>
class SerialQueue {
public:
    // 构造函数
    SerialQueue() : head_(nullptr), tail_(nullptr), size_(0) {}

    // 析构函数
    ~SerialQueue() {
        while (head_ != nullptr) {
            Node* temp = head_->next;
            delete head_;
            head_ = temp;
        }
    }

    // 将元素x入队至队列尾部
    void enqueue(const T& x) {
        Node* newNode = new Node{x, nullptr};
        if (tail_ == nullptr) {
            // 队列为空时
            head_ = tail_ = newNode;
        } else {
            // 队列非空时
            tail_->next = newNode;
            tail_ = newNode;
        }
        ++size_;
    }

    // 从队列头部出队一个元素，返回出队元素的值
    bool dequeue(T* ret) {
        if (head_ == nullptr) {
            return false; // 队列为空时返回false
        }
        Node* temp = head_;
        *ret = head_->data; // 通过指针返回数据
        head_ = head_->next;
        if (head_ == nullptr) {
            tail_ = nullptr; // 如果队列变空，尾指针也置为nullptr
        }
        delete temp; // 释放原头节点内存
        --size_;
        return true;
    }

    // 返回队列大小
    int size() const { return size_; }

private:
    struct Node {
        T data;
        Node* next;
        Node(const T& data, Node* next) : data(data), next(next) {}
    };

    Node* head_; // 指向队列头部的指针
    Node* tail_; // 指向队列尾部的指针
    int size_; // 队列中的元素数量
};

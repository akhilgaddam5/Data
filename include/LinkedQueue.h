#pragma once
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

// Singly linked FIFO queue with unique_ptr ownership (RAII).
// Each node owns its successor; destroying the queue frees the whole chain
// automatically. back_ is a non-owning raw pointer to the last node.
// Demonstrates: encapsulation, the rule of five, and pointer/ownership skills.
template <typename T>
class LinkedQueue {
public:
    LinkedQueue() = default;

    LinkedQueue(const LinkedQueue& other)   // deep-copy constructor
        : front_(clone(other.front_)) {
        back_ = front_ ? tailOf(front_.get()) : nullptr;
    }

    LinkedQueue& operator=(const LinkedQueue& other) {  // copy assignment
        if (this != &other) {
            LinkedQueue tmp(other);
            swap(tmp);
        }
        return *this;
    }

    LinkedQueue(LinkedQueue&&) noexcept = default;       // move constructor
    LinkedQueue& operator=(LinkedQueue&&) noexcept = default;
    ~LinkedQueue() = default;                            // RAII: chain freed

    void enqueue(const T& value) {
        auto node = std::make_unique<Node>(value);
        Node* raw = node.get();
        if (empty())
            front_ = std::move(node);
        else
            back_->next = std::move(node);
        back_ = raw;
    }

    T dequeue() {
        if (empty()) throw std::underflow_error("dequeue on empty queue");
        T value = std::move(front_->data);
        front_ = std::move(front_->next);
        if (!front_) back_ = nullptr;
        return value;
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("front on empty queue");
        return front_->data;
    }

    bool empty() const noexcept { return front_ == nullptr; }
    std::size_t size() const {
        std::size_t n = 0;
        for (const Node* p = front_.get(); p; p = p->next.get()) ++n;
        return n;
    }

private:
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        explicit Node(const T& value) : data(value) {}
    };

    static std::unique_ptr<Node> clone(const std::unique_ptr<Node>& list) {
        if (!list) return nullptr;
        auto head = std::make_unique<Node>(list->data);
        Node* writer = head.get();
        for (const Node* reader = list->next.get(); reader; reader = reader->next.get()) {
            writer->next = std::make_unique<Node>(reader->data);
            writer = writer->next.get();
        }
        return head;
    }

    static Node* tailOf(Node* list) {
        while (list->next) list = list->next.get();
        return list;
    }

    void swap(LinkedQueue& other) noexcept {
        std::swap(front_, other.front_);
        std::swap(back_, other.back_);
    }

    std::unique_ptr<Node> front_;  // owns the entire node chain
    Node* back_ = nullptr;        // non-owning pointer to last node
};

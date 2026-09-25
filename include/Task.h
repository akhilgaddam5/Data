#pragma once
#include <ostream>
#include <string>
#include <utility>

// Domain entity: a data-center maintenance task.
// Encapsulation + operator overloading (used by every collection below).
class Task {
public:
    Task(int id, int priority, std::string description)
        : id_(id), priority_(priority), description_(std::move(description)) {}

    int id() const { return id_; }
    int priority() const { return priority_; }
    const std::string& description() const { return description_; }

    // Ordering by priority drives BST placement and every sort algorithm.
    bool operator<(const Task& other) const { return priority_ < other.priority_; }
    bool operator>(const Task& other) const { return priority_ > other.priority_; }
    // Identity drives BST search/removal.
    bool operator==(const Task& other) const { return id_ == other.id_; }

    friend std::ostream& operator<<(std::ostream& os, const Task& task) {
        return os << "[#" << task.id_ << " | p" << task.priority_ << " | "
                  << task.description_ << "]";
    }

private:
    int id_;
    int priority_;
    std::string description_;
};

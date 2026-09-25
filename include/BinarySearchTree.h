#pragma once
#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include "LinkedQueue.h"

template <typename T>
struct BSTNode {
    T data;
    std::unique_ptr<BSTNode<T>> left;
    std::unique_ptr<BSTNode<T>> right;
    explicit BSTNode(const T& value) : data(value) {}
};

// Binary Search Tree with unique_ptr ownership (RAII): nodes are freed
// automatically when the tree is destroyed; no manual delete anywhere.
// in-order traversal yields sorted order; level-order uses LinkedQueue (BFS).
template <typename T>
class BinarySearchTree {
public:
    void insert(const T& value) { insert(root_, value); ++count_; }

    bool contains(const T& value) const { return contains(root_, value); }

    bool remove(const T& value) {
        if (!contains(value)) return false;
        remove(root_, value);
        --count_;
        return true;
    }

    std::vector<T> inOrder() const {
        std::vector<T> out;
        out.reserve(count_);
        inOrder(root_, out);
        return out;
    }

    std::vector<T> levelOrder() const {
        std::vector<T> out;
        if (root_) {
            LinkedQueue<const BSTNode<T>*> queue;
            queue.enqueue(root_.get());
            while (!queue.empty()) {
                const BSTNode<T>* node = queue.dequeue();
                out.push_back(node->data);
                if (node->left) queue.enqueue(node->left.get());
                if (node->right) queue.enqueue(node->right.get());
            }
        }
        return out;
    }

    int height() const { return height(root_) - 1; }  // empty tree -> -1
    std::size_t size() const { return count_; }
    bool empty() const { return count_ == 0; }

private:
    std::unique_ptr<BSTNode<T>> root_;
    std::size_t count_ = 0;

    static void insert(std::unique_ptr<BSTNode<T>>& node, const T& value) {
        if (!node) {
            node = std::make_unique<BSTNode<T>>(value);
            return;
        }
        if (value < node->data)
            insert(node->left, value);
        else
            insert(node->right, value);
    }

    static bool contains(const std::unique_ptr<BSTNode<T>>& node, const T& value) {
        if (!node) return false;
        if (value == node->data) return true;
        return value < node->data ? contains(node->left, value)
                                  : contains(node->right, value);
    }

    static void remove(std::unique_ptr<BSTNode<T>>& node, const T& value) {
        if (!node) return;
        if (value < node->data) {
            remove(node->left, value);
        } else if (node->data < value) {
            remove(node->right, value);
        } else if (!node->left && !node->right) {
            node.reset();                                   // leaf: free node
        } else if (!node->left) {
            node = std::move(node->right);                  // one child
        } else if (!node->right) {
            node = std::move(node->left);                   // one child
        } else {
            const BSTNode<T>* succ = node->right.get();     // two children:
            while (succ->left) succ = succ->left.get();     // in-order successor
            const T succData = succ->data;
            node->data = succData;
            remove(node->right, succData);
        }
    }

    static void inOrder(const std::unique_ptr<BSTNode<T>>& node, std::vector<T>& out) {
        if (!node) return;
        inOrder(node->left, out);
        out.push_back(node->data);
        inOrder(node->right, out);
    }

    static int height(const std::unique_ptr<BSTNode<T>>& node) {
        if (!node) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    }
};

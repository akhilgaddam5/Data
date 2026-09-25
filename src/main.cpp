#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <vector>

#include "BinarySearchTree.h"
#include "LinkedQueue.h"
#include "Sorters.h"
#include "Task.h"

namespace {

void printInts(const std::string& label, const std::vector<int>& values) {
    std::cout << label;
    for (std::size_t i = 0; i < values.size(); ++i)
        std::cout << values[i] << (i + 1 < values.size() ? " " : "");
    std::cout << "\n";
}

void printTasks(const std::string& label, const std::vector<Task>& tasks) {
    std::cout << label << "\n";
    for (const Task& t : tasks) std::cout << "  " << t << "\n";
}

std::vector<int> randomInts(int count) {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 99);
    std::vector<int> values(static_cast<std::size_t>(count));
    for (int& v : values) v = dist(rng);
    return values;
}

}  // namespace

int main() {
    // ---- 1. Tree collection: BST keyed on task priority ----
    std::cout << "=== 1. Binary Search Tree (ordered by priority) ===\n";
    BinarySearchTree<Task> tree;
    const std::vector<Task> seed = {
        Task(101, 5, "Replace failed DIMM in rack B12"),
        Task(102, 1, "Firmware patch on top-of-rack switch"),
        Task(103, 8, "Coolant leak sensor alarm, hall 2"),
        Task(104, 3, "Cable audit for row C"),
        Task(105, 7, "Patch panel rewire, cage 4"),
        Task(106, 2, "IPMI password rotation"),
        Task(107, 9, "PDU breaker overload, aisle 7"),
        Task(108, 4, "Label and rack new 2U backup node"),
    };
    for (const Task& t : seed) tree.insert(t);

    std::cout << "Size: " << tree.size() << ", height: " << tree.height() << "\n";
    printTasks("In-order traversal (sorted by priority):", tree.inOrder());
    printTasks("Level-order traversal (BFS via linked queue):", tree.levelOrder());

    tree.remove(Task(103, 8, ""));
    printTasks("After removing task #103:", tree.inOrder());
    std::cout << "\n";

    // ---- 2. Queue collection: FIFO work order ----
    std::cout << "=== 2. Linked Queue (FIFO work order) ===\n";
    LinkedQueue<Task> workOrder;
    workOrder.enqueue(Task(201, 4, "Ack temperature alert"));
    workOrder.enqueue(Task(202, 3, "Swap transceiver on port 24"));
    workOrder.enqueue(Task(203, 6, "Escalate failed node to vendor"));
    std::cout << "Queued " << workOrder.size() << " tasks, processing FIFO:\n";
    while (!workOrder.empty())
        std::cout << "  done -> " << workOrder.dequeue() << "\n";
    std::cout << "\n";

    // ---- 3. Sorting algorithms (polymorphic Strategy) on ints ----
    std::cout << "=== 3. Sorting algorithms on integers ===\n";
    const std::vector<int> numbers = randomInts(15);
    printInts("Unsorted:  ", numbers);

    std::vector<int> reference = numbers;
    std::sort(reference.begin(), reference.end());

    std::vector<std::unique_ptr<Sorter<int>>> intSorters;
    intSorters.push_back(std::make_unique<MergeSort<int>>());
    intSorters.push_back(std::make_unique<QuickSort<int>>());
    intSorters.push_back(std::make_unique<HeapSort<int>>());

    for (const auto& sorter : intSorters) {
        std::vector<int> copy = numbers;
        sorter->sort(copy);
        printInts(sorter->name() + ":  ", copy);
        std::cout << "  matches std::sort: " << (copy == reference ? "yes" : "NO") << "\n";
    }

    // Cross-check: inserting the same numbers into a BST and reading in-order
    // must equal the sorted vector.
    BinarySearchTree<int> intTree;
    for (int v : numbers) intTree.insert(v);
    std::cout << "BST in-order equals sorted vector: "
              << (intTree.inOrder() == reference ? "yes" : "NO") << "\n\n";

    // ---- 4. Sorting Tasks + heap-backed priority queue ----
    std::cout << "=== 4. Sorting Tasks and priority queue ===\n";
    const std::vector<Task> tasks = seed;
    std::vector<std::unique_ptr<Sorter<Task>>> taskSorters;
    taskSorters.push_back(std::make_unique<MergeSort<Task>>());
    taskSorters.push_back(std::make_unique<QuickSort<Task>>());
    taskSorters.push_back(std::make_unique<HeapSort<Task>>());
    for (const auto& sorter : taskSorters) {
        std::vector<Task> copy = tasks;
        sorter->sort(copy);
        printTasks(sorter->name() + " (by priority):", copy);
    }

    // STL priority_queue: heap-backed collection, highest priority served first.
    struct HigherPriorityFirst {
        bool operator()(const Task& a, const Task& b) const {
            return a.priority() < b.priority();
        }
    };
    std::priority_queue<Task, std::vector<Task>, HigherPriorityFirst> pq;
    for (const Task& t : tasks) pq.push(t);
    std::cout << "Priority queue drain (highest priority first):\n";
    while (!pq.empty()) {
        std::cout << "  " << pq.top() << "\n";
        pq.pop();
    }
    return 0;
}

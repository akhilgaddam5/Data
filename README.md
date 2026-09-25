# Data Center Collections — C++ OOP Project

A modern C++17 project demonstrating object-oriented programming, custom
collections (binary search tree, linked queue), and sorting algorithms,
built around a data-center task management domain.

## Project Structure

```
include/
  Task.h              Domain entity: encapsulation + operator overloading
  LinkedQueue.h       Linked FIFO queue: RAII, rule of five
  BinarySearchTree.h  BST: unique_ptr ownership, traversals
  Sorter.h            Abstract Strategy interface (pure virtual)
  Sorters.h           MergeSort, QuickSort, HeapSort implementations
src/
  main.cpp            Demo tying all collections and algorithms together
CMakeLists.txt        Build configuration
```

## OOP Principles Demonstrated

| Principle | Where |
|-----------|-------|
| Encapsulation | `Task` hides its fields behind accessors; `LinkedQueue` hides its node chain |
| Abstraction | `Sorter<T>` is a pure virtual interface — callers never see algorithm internals |
| Inheritance | `MergeSort`, `QuickSort`, `HeapSort` derive from `Sorter<T>` |
| Polymorphism | `sorter->sort(copy)` dispatches at runtime (Strategy pattern) |
| Templates | Every collection and sorter is generic (`Task`, `int`, raw pointers) |
| Operator overloading | `Task` overloads `<`, `>`, `==`, `<<` |
| RAII | `unique_ptr` owns every tree node and queue node; zero manual `delete` |
| Rule of five | `LinkedQueue` defines copy/move constructors and assignments |

## Collections

- **BinarySearchTree** — insert, search, remove (all three node cases including
  two-children with in-order successor), in-order traversal (sorted output),
  level-order traversal (BFS, powered by the queue), height, size.
- **LinkedQueue** — linked FIFO queue with `unique_ptr` ownership chain and a
  non-owning raw tail pointer; deep-copy semantics; used for BFS and work orders.
- **std::priority_queue** — heap-backed STL collection used to drain tasks by
  highest priority.

## Sorting Algorithms

| Algorithm | Complexity | Properties |
|-----------|------------|------------|
| Merge Sort | O(n log n) guaranteed | Stable, O(n) extra space |
| Quick Sort | O(n log n) average | In-place, median-of-three pivot, Hoare partition |
| Heap Sort | O(n log n) guaranteed | In-place, max-heap sift-down |

Each sorter's output is verified against `std::sort` at runtime, and the BST
in-order traversal is cross-checked against the sorted vector.

## Build and Run

Requires a C++17 compiler (g++ 7+, clang 5+, or MSVC 2017+).

```bash
# Option A: CMake
cmake -B build && cmake --build build
./build/dccollections

# Option B: single command
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp -o dccollections
./dccollections
```

## Suggested Exercises

- Add an AVL or red-black tree that inherits from a shared tree interface
- Add `BubbleSort` and `InsertionSort` as new `Sorter` strategies
- Add an iterator class so `BinarySearchTree` works with range-based for loops

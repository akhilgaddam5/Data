#pragma once
#include <string>
#include <vector>

// Abstract base class with a pure virtual interface: the Strategy pattern.
// Polymorphism: callers sort through Sorter<T>& without knowing which
// algorithm runs underneath. Abstraction: no implementation details here.
template <typename T>
class Sorter {
public:
    virtual ~Sorter() = default;
    virtual std::string name() const = 0;
    virtual void sort(std::vector<T>& data) const = 0;
};

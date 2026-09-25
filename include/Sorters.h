#pragma once
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "Sorter.h"

// O(n log n) guaranteed, stable, O(n) extra space.
template <typename T>
class MergeSort : public Sorter<T> {
public:
    std::string name() const override { return "Merge Sort"; }
    void sort(std::vector<T>& data) const override {
        if (data.size() > 1) mergeSort(data, 0, data.size());
    }

private:
    static void mergeSort(std::vector<T>& a, std::size_t lo, std::size_t hi) {
        if (hi - lo < 2) return;
        std::size_t mid = lo + (hi - lo) / 2;
        mergeSort(a, lo, mid);
        mergeSort(a, mid, hi);
        merge(a, lo, mid, hi);
    }

    static void merge(std::vector<T>& a, std::size_t lo, std::size_t mid, std::size_t hi) {
        std::vector<T> left(a.begin() + lo, a.begin() + mid);
        std::vector<T> right(a.begin() + mid, a.begin() + hi);
        std::size_t i = 0, j = 0, k = lo;
        while (i < left.size() && j < right.size())
            a[k++] = (right[j] < left[i]) ? right[j++] : left[i++];
        while (i < left.size()) a[k++] = left[i++];
        while (j < right.size()) a[k++] = right[j++];
    }
};

// O(n log n) average, in-place, unstable. Median-of-three pivot + Hoare partition.
template <typename T>
class QuickSort : public Sorter<T> {
public:
    std::string name() const override { return "Quick Sort"; }
    void sort(std::vector<T>& data) const override {
        if (!data.empty())
            quickSort(data, 0, static_cast<int>(data.size()) - 1);
    }

private:
    static void quickSort(std::vector<T>& a, int lo, int hi) {
        if (lo >= hi) return;
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < a[lo]) std::swap(a[lo], a[mid]);   // median-of-three pivot
        if (a[hi] < a[lo]) std::swap(a[lo], a[hi]);
        if (a[hi] < a[mid]) std::swap(a[mid], a[hi]);
        const T pivot = a[mid];
        int i = lo - 1, j = hi + 1;
        while (true) {
            do { ++i; } while (a[i] < pivot);
            do { --j; } while (pivot < a[j]);
            if (i >= j) break;
            std::swap(a[i], a[j]);
        }
        quickSort(a, lo, j);
        quickSort(a, j + 1, hi);
    }
};

// O(n log n) guaranteed, in-place, unstable. Max-heap + sift-down.
template <typename T>
class HeapSort : public Sorter<T> {
public:
    std::string name() const override { return "Heap Sort"; }
    void sort(std::vector<T>& data) const override {
        const std::size_t n = data.size();
        if (n < 2) return;
        for (std::size_t start = (n - 2) / 2 + 1; start-- > 0;)
            siftDown(data, start, n - 1);
        for (std::size_t end = n - 1; end > 0; --end) {
            std::swap(data[0], data[end]);
            siftDown(data, 0, end - 1);
        }
    }

private:
    static void siftDown(std::vector<T>& a, std::size_t root, std::size_t end) {
        while (true) {
            std::size_t child = 2 * root + 1;
            if (child > end) return;
            if (child + 1 <= end && a[child] < a[child + 1]) ++child;
            if (a[root] < a[child]) {
                std::swap(a[root], a[child]);
                root = child;
            } else {
                return;
            }
        }
    }
};

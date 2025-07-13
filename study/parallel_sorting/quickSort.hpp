#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <functional>

template<typename T, typename Compare>
int partition(std::vector<T>& arr, int low, int high, Compare comp) {
    T pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (comp(arr[j], pivot)) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

template<typename T, typename Compare = std::less<T>>
void quickSort(std::vector<T>& arr, int low, int high, Compare comp = Compare()) {
    std::stack<std::pair<int, int>> stack;
    stack.push({low, high});

    while (!stack.empty()) {
        auto [l, h] = stack.top();
        stack.pop();

        if (l < h) {
            int p = partition(arr, l, h, comp);
            if (p + 1 < h)
                stack.push({p + 1, h});
            if (l < p - 1)
                stack.push({l, p - 1});
        }
    }
}

template<typename T>
void printVector(const std::vector<T>& arr) {
    for (const auto& el : arr)
        std::cout << el << " ";
    std::cout << "\n";
}


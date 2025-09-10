#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


template<typename T>
int linear_search(const T* arr, int size, const T& target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1; 
}


template<typename T>
int linear_search(const std::vector<T>& vec, const T& target) {
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1; 
}


template<typename T>
std::vector<T> generate_random_array(int size, T max_value) {
    std::vector<T> arr(size);
    
    srand(static_cast<unsigned>(time(nullptr)));
    
    for (int i = 0; i < size; i++) {
        arr[i] = static_cast<T>(rand() % max_value + 1);
    }
    
    return arr;
}


int generate_random_size(int min_size = 30, int max_size = 100) {
    srand(static_cast<unsigned>(time(nullptr)));
    return rand() % (max_size - min_size + 1) + min_size;
}


template<typename T>
void print_array(const std::vector<T>& arr) {
    std::cout << "Массив: ";
    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << arr[i];
        if (i < arr.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

template<typename T>
void demonstrate_search(const std::vector<T>& arr, const T& target) {
    std::cout << "Размер массива: " << arr.size() << std::endl;
    print_array(arr);
    
    std::cout << "Ищем значение: " << target << std::endl;
    
    int index = linear_search(arr, target);
    
    if (index != -1) {
        std::cout << "Значение " << target << " найдено на позиции: " << index << std::endl;
    } else {
        std::cout << "Значение " << target << " не найдено в массиве" << std::endl;
    }
}

#endif // LINEAR_SEARCH_H

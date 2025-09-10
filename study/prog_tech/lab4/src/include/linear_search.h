#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <cstring>
#include <string>


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


template<>
int linear_search<char*>(const std::vector<char*>& vec, char* const& target) {
    for (size_t i = 0; i < vec.size(); i++) {
        if (strcmp(vec[i], target) == 0) {
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


template<>
std::vector<char> generate_random_array<char>(int size, char max_value) {
    std::vector<char> arr(size);
    
    srand(static_cast<unsigned>(time(nullptr)));
    
    for (int i = 0; i < size; i++) {

        if (max_value >= 'A' && max_value <= 'Z') {
            arr[i] = 'A' + (rand() % (max_value - 'A' + 1));
        } else {
            arr[i] = '0' + (rand() % 10); 
        }
    }
    
    return arr;
}


template<>
std::vector<float> generate_random_array<float>(int size, float max_value) {
    std::vector<float> arr(size);
    
    srand(static_cast<unsigned>(time(nullptr)));
    
    for (int i = 0; i < size; i++) {
        arr[i] = static_cast<float>(rand()) / RAND_MAX * max_value;
    }
    
    return arr;
}


template<>
std::vector<double> generate_random_array<double>(int size, double max_value) {
    std::vector<double> arr(size);
    
    srand(static_cast<unsigned>(time(nullptr)));
    
    for (int i = 0; i < size; i++) {
        arr[i] = static_cast<double>(rand()) / RAND_MAX * max_value;
    }
    
    return arr;
}


template<>
std::vector<char*> generate_random_array<char*>(int size, char* max_value) {
    std::vector<char*> arr(size);
    

    const char* words[] = {"apple", "banana", "cherry", "date", "elderberry", 
                          "fig", "grape", "honey", "ice", "jam",
                          "kiwi", "lemon", "mango", "nut", "orange",
                          "peach", "quince", "raisin", "strawberry", "tomato"};
    int word_count = sizeof(words) / sizeof(words[0]);
    
    srand(static_cast<unsigned>(time(nullptr)));
    
    for (int i = 0; i < size; i++) {
        const char* selected_word = words[rand() % word_count];
        arr[i] = new char[strlen(selected_word) + 1];
        strcpy(arr[i], selected_word);
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


template<>
void demonstrate_search<char*>(const std::vector<char*>& arr, char* const& target) {
    std::cout << "Размер массива: " << arr.size() << std::endl;
    print_array(arr);
    
    std::cout << "Ищем значение: " << target << std::endl;
    
    int index = linear_search(arr, target);
    
    if (index != -1) {
        std::cout << "Значение \"" << target << "\" найдено на позиции: " << index << std::endl;
    } else {
        std::cout << "Значение \"" << target << "\" не найдено в массиве" << std::endl;
    }
}


void cleanup_char_array(std::vector<char*>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        delete[] arr[i];
    }
    arr.clear();
}

#endif // LINEAR_SEARCH_H

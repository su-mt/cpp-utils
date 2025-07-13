#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <type_traits>
#include <list>
#include <cmath>
#include <algorithm>
#include <queue>
#include <optional>
#include <functional>
#include <cstdio>

#include "ThreadPool.hpp"

extern const unsigned max_threads;

// ---------- Merge Sort for vector with custom comparator ----------

template<typename T, typename Compare = std::less<T>>
std::vector<T> merge(const std::vector<T>& left, const std::vector<T>& right, Compare comp = Compare()) {
    std::vector<T> result;
    size_t i = 0, j = 0;
    result.reserve(left.size() + right.size());

    while (i < left.size() && j < right.size()) {
        if (!comp(right[j], left[i]))  // left[i] <= right[j]
            result.push_back(left[i++]);
        else
            result.push_back(right[j++]);
    }
    while (i < left.size()) result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);
    return result;
}

template<typename T, typename Compare = std::less<T>>
std::vector<T> mergeSort(std::vector<T>&& input, Compare comp = Compare()) {
    if (input.size() <= 1)
        return std::move(input);

    std::vector<T> buffer(input.size());
    std::vector<T>* src = &input;
    std::vector<T>* dst = &buffer;

    for (size_t width = 1; width < input.size(); width *= 2) {
        for (size_t i = 0; i < input.size(); i += 2 * width) {
            size_t left = i;
            size_t mid = std::min(i + width, input.size());
            size_t right = std::min(i + 2 * width, input.size());

            size_t a = left, b = mid, idx = left;
            while (a < mid && b < right) {
                if (!comp((*src)[b], (*src)[a]))  // (*src)[a] <= (*src)[b]
                    (*dst)[idx++] = std::move((*src)[a++]);
                else
                    (*dst)[idx++] = std::move((*src)[b++]);
            }
            while (a < mid) (*dst)[idx++] = std::move((*src)[a++]);
            while (b < right) (*dst)[idx++] = std::move((*src)[b++]);
        }
        std::swap(src, dst);
    }
    return std::move(*src);
}

// ---------- Чтение и запись ----------

template<typename T>
constexpr bool is_number = std::is_arithmetic<T>::value;

template<typename T>
bool writeChunk(const std::string& filename, const std::vector<T>& data) {
    std::ofstream out(filename);
    if (!out) return false;
    for (const auto& item : data) out << item << "\n";
    return true;
}

template<typename T>
bool readChunk(const std::string& filename, std::vector<T>& data) {
    std::ifstream in(filename);
    if (!in) return false;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if constexpr (is_number<T>) {
            std::istringstream iss(line);
            T value;
            iss >> value;
            if (!iss.fail()) data.push_back(value);
        } else {
            data.push_back(line);
        }
    }
    return true;
}

template<typename T, typename Compare = std::less<T>>
void processChunk(const std::string& path, Compare comp = Compare()) {
    std::vector<T> data;
    if (!readChunk<T>(path, data)) {
        std::cerr << "Ошибка чтения: " << path << "\n";
        return;
    }
    std::cout << "Чанк считан: " << path << " (" << data.size() << " элементов)\n";

    auto sorted = mergeSort<T, Compare>(std::move(data), comp);

    if (!writeChunk<T>(path, sorted)) {
        std::cerr << "Ошибка записи: " << path << "\n";
    } else {
        std::cout << "Отсортирован: " << path << "\n";
    }
}

// ---------- Параллельная обработка чанков ----------

template<typename T, typename Compare = std::less<T>>
void sortAllChunks(const std::string& baseFilename, int totalChunks, Compare comp = Compare()) {
    ThreadPool pool(std::min(std::thread::hardware_concurrency(), max_threads));
    std::atomic<int> completed{0};
    for (int i = 0; i < totalChunks; ++i) {
        std::string chunk = baseFilename + ".part" + std::to_string(i);
        pool.enqueue([chunk, &completed, comp]() mutable {
            processChunk<T, Compare>(chunk, comp);
            ++completed;
        });
    }
    while (completed < totalChunks) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Все чанки отсортированы." << std::endl;
}

// Макрос для логирования
#define LOG(x) std::cout << x << std::endl

// ---------- Структура для кучи ----------

template<typename T>
struct HeapNode {
    T value;
    size_t chunkIndex;
};

template<typename T, typename Compare = std::less<T>>
bool mergeChunksToFile(const std::string& baseFilename,
                       int totalChunks,
                       const std::string& outFile,
                       Compare comp = Compare()) {
    using Node = HeapNode<T>;
    auto cmp = [&](const Node& a, const Node& b) { return comp(b.value, a.value); };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> heap(cmp);

    std::vector<std::ifstream> inputs(totalChunks);
    std::vector<bool> alive(totalChunks, true);
    std::vector<std::string> names(totalChunks);

    // Инициализация
    for (int i = 0; i < totalChunks; ++i) {
        names[i] = baseFilename + ".part" + std::to_string(i);
        inputs[i].open(names[i]);
        if (!inputs[i]) {
            std::cerr << "не удалось открыть: " << names[i] << std::endl;
            return false;
        }
        std::string line;
        if (std::getline(inputs[i], line) && !line.empty()) {
            std::istringstream iss(line);
            T val;
            if constexpr (is_number<T>) iss >> val, heap.push({val, (size_t)i});
            else heap.push({line, (size_t)i});
        } else alive[i] = false;
    }

    std::ofstream out(outFile);
    if (!out) { std::cerr << "Не удалось открыть выходной файл: " << outFile << std::endl; return false; }

    size_t written = 0;
    while (!heap.empty()) {
        auto cur = heap.top(); heap.pop();
        out << cur.value << "\n";
        ++written;
        if (written % 1'000'000 == 0) LOG("Прогресс: " << written);

        size_t idx = cur.chunkIndex;
        if (!alive[idx]) continue;

        std::string line;
        if (std::getline(inputs[idx], line) && !line.empty()) {
            std::istringstream iss(line);
            T val;
            if constexpr (is_number<T>) {
                if (!(iss >> val)) { std::cerr << "Ошибка парсинга в " << names[idx] << std::endl; }
                else heap.push({val, idx});
            } else {
                heap.push({line, idx});
            }
        } else {
            inputs[idx].close();
            alive[idx] = false;
            if (std::remove(names[idx].c_str()) != 0)
                std::cerr << "⚠️ Не удалось удалить " << names[idx] << std::endl;
            else LOG("Удалён: " << names[idx]);
        }
    }
    LOG("Слияние завершено: " << outFile << " (" << written << " строк)");
    return true;
}

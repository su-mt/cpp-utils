#pragma once

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
#include <iomanip>            
#include <limits>

#include "ThreadPool.hpp"

namespace fs = std::filesystem;


extern const unsigned max_threads;



// ---------- Чтение и запись ----------

template<typename T>
constexpr bool is_number = std::is_arithmetic<T>::value;

template<typename T>
bool writeChunk(const std::string& filename, const std::vector<T>& data) {
    std::ofstream out(filename);
    if (!out) return false;

    // Для плавающей точки настраиваем фиксированный формат и максимальную точность
    if constexpr (std::is_floating_point_v<T>) {
        out << std::fixed
            << std::setprecision(std::numeric_limits<T>::max_digits10);
    }

    if constexpr (std::is_floating_point_v<T>) {
    std::cout << "[DEBUG] formatting floats with precision = "
              << std::numeric_limits<T>::max_digits10 << "\n";
    out << std::fixed
        << std::setprecision(std::numeric_limits<T>::max_digits10);
    }


    for (const auto& item : data) {
        out << item << "\n";
    }
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

// Размер чанка в байтах (100 МБ)
constexpr std::uint64_t CHUNK_SIZE = 100ULL * 1024 * 1024;

bool openFile(const std::string& filename, std::ifstream& in) {
    in.open(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл \"" << filename << "\"\n";
        return false;
    }
    return true;
}

std::uint64_t countChunks(const fs::path& path) {
    const auto fileSize = fs::file_size(path);
    return (fileSize + CHUNK_SIZE - 1) / CHUNK_SIZE;  // округление вверх
}

int splitIntoChunks(const std::string& filename) {
    std::ifstream in;
    if (!openFile(filename, in)) {
        std::exit(EXIT_FAILURE);
    }

    fs::path filepath{filename};
    std::uint64_t totalChunks = countChunks(filepath);
    std::cout << "Всего чанков: " << totalChunks << "\n";

    for (std::uint64_t chunkIndex = 0; chunkIndex < totalChunks; ++chunkIndex) {
        // Формируем имя для чанка, например: filename.part0, .part1 и т.д.
        std::string outName = filename + ".part" + std::to_string(chunkIndex);
        std::ofstream out(outName, std::ios::binary);
        if (!out) {
            std::cerr << "Ошибка: не удалось создать файл-чанк \"" << outName << "\"\n";
            break;
        }

        // Сколько байт читать в этом чанке
        std::uint64_t bytesToRead = CHUNK_SIZE;
        // Для последнего чанка может быть меньше
        const auto currentPos = chunkIndex * CHUNK_SIZE;
        const auto fileSize = fs::file_size(filepath);
        if (currentPos + CHUNK_SIZE > fileSize) {
            bytesToRead = fileSize - currentPos;
        }

        // Буфер и чтение/запись
        std::vector<char> buffer(4096);
        std::uint64_t bytesReadTotal = 0;
        while (bytesReadTotal < bytesToRead && in) {
            std::uint64_t toRead = std::min<std::uint64_t>(buffer.size(), bytesToRead - bytesReadTotal);
            in.read(buffer.data(), toRead);
            std::streamsize n = in.gcount();
            if (n <= 0) break;
            out.write(buffer.data(), n);
            bytesReadTotal += static_cast<std::uint64_t>(n);
        }
        out.close();
        std::cout << "Создан: " << outName << " (" << bytesReadTotal << " байт)\n";
    }

    in.close();

    return totalChunks;
}


template<typename T>
int splitIntoChunks(const std::string& filename) {
    static_assert(std::is_arithmetic_v<T>, "Тип должен быть числовым");

    // Считаем все данные из файла
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Ошибка: не удалось открыть файл \"" << filename << "\"\n";
        std::exit(EXIT_FAILURE);
    }

    // Собираем все значения в вектор
    std::vector<T> data;
    T value;
    while (in >> value) {
        data.push_back(value);
    }
    in.close();

    // Сколько элементов в одном чанке
    const std::size_t elementsPerChunk = CHUNK_SIZE / sizeof(T);
    // Сколько всего чанков
    int totalChunks = static_cast<int>((data.size() + elementsPerChunk - 1) / elementsPerChunk);
    std::cout << "Всего чанков: " << totalChunks << "\n";

    // Разбиваем и пишем
    for (int chunkIndex = 0; chunkIndex < totalChunks; ++chunkIndex) {
        size_t start = chunkIndex * elementsPerChunk;
        size_t end   = std::min(start + elementsPerChunk, data.size());

        std::string outName = filename + ".part" + std::to_string(chunkIndex);

        // writeChunk<T> пишет каждый элемент в отдельную строку
        if (!writeChunk<T>(outName, std::vector<T>(data.begin() + start, data.begin() + end))) {
            std::cerr << "Ошибка: не удалось создать файл-чанк \"" << outName << "\"\n";
        } else {
            std::cout << "Создан: " << outName
                      << " (" << (end - start) << " элементов)\n";
        }
    }

    return totalChunks;
}



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

    // Если T — плавающий, настраиваем фиксированный формат и максимальную точность
    if constexpr (std::is_floating_point_v<T>) {
        out << std::fixed
            << std::setprecision(std::numeric_limits<T>::max_digits10);
    }


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


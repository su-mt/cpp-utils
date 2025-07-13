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
#include <cstdio>

#include "ThreadPool.hpp"

extern const unsigned max_threads;

// ---------- Merge Sort for vector ----------
template<typename T>
std::vector<T> merge(const std::vector<T>& left, const std::vector<T>& right) {
    std::vector<T> result;
    size_t i = 0, j = 0;
    result.reserve(left.size() + right.size());

    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j])
            result.push_back(left[i++]);
        else
            result.push_back(right[j++]);
    }

    while (i < left.size()) result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);

    return result;
}

template<typename T>
std::vector<T> mergeSort(std::vector<T>&& input) {
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
                if ((*src)[a] <= (*src)[b])
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
    if (!out.is_open()) return false;

    for (const auto& item : data) {
        out << item << "\n";
    }
    return true;
}

template<typename T>
bool readChunk(const std::string& filename, std::vector<T>& data) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) {
        if constexpr (is_number<T>) {
            std::istringstream iss(line);
            T value;
            iss >> value;
            if (iss.fail()) continue;
            data.push_back(value);
        } else {
            data.push_back(line);
        }
    }
    return true;
}

template<typename T>
void processChunk(const std::string& path) {
    std::vector<T> data;

    if (!readChunk<T>(path, data)) {
        std::cerr << "Ошибка чтения: " << path << "\n";
        return;
    }

    std::cout << "Чанк считан: " << path << " (" << data.size() << " элементов)\n";

    data = mergeSort<T>(std::move(data));

    if (!writeChunk<T>(path, data)) {
        std::cerr << "Ошибка записи: " << path << "\n";
    } else {
        std::cout << "Отсортирован: " << path << "\n";
    }
}


template<typename T>
void sortAllChunks(const std::string& baseFilename, int totalChunks) {
    ThreadPool pool(std::min(std::thread::hardware_concurrency(), static_cast<unsigned int>(max_threads)));
    std::atomic<int> completedChunks{0};

    for (int i = 0; i < totalChunks; ++i) {
        std::ostringstream oss;
        oss << baseFilename << ".part" << i;
        std::string chunkFile = oss.str();

        pool.enqueue([chunkFile, &completedChunks]() {
            processChunk<T>(chunkFile);
            ++completedChunks;
        });
    }

    // Ждём завершения всех заданий
    while (completedChunks < totalChunks) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Все чанки отсортированы.\n";
}




// Макрос для логирования (можно закомментировать для тишины)
#define LOG(x) std::cout << x << "\n"

// ---------- Структура для кучи ----------
template<typename T>
struct HeapNode {
    T value;
    size_t chunkIndex;

    bool operator>(const HeapNode<T>& other) const {
        return value > other.value;
    }
};


template<typename T>
bool mergeChunksToFile(const std::string& baseFilename, int totalChunks, const std::string& outputFilename) {
    using Node = HeapNode<T>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> minHeap;

    std::vector<std::ifstream> inputs(totalChunks);
    std::vector<bool> validChunk(totalChunks, false);
    std::vector<std::string> partFilenames(totalChunks); // хранение имён для удаления

    // ---------- Инициализация входных файлов ----------
    for (int i = 0; i < totalChunks; ++i) {
        std::ostringstream path;
        path << baseFilename << ".part" << i;
        partFilenames[i] = path.str();

        inputs[i].open(partFilenames[i]);
        if (!inputs[i].is_open()) {
            std::cerr << "не удалось открыть чанк: " << partFilenames[i] << "\n";
            return false;
        }

        std::string line;
        while (std::getline(inputs[i], line)) {
            if (line.empty()) continue;

            std::istringstream iss(line);
            T value;

            if constexpr (is_number<T>) {
                iss >> value;
                if (iss.fail()) {
                    std::cerr << "Ошибка парсинга в " << partFilenames[i] << ": " << line << "\n";
                    continue;
                }
            } else {
                value = line;
            }

            minHeap.push({value, static_cast<size_t>(i)});
            validChunk[i] = true;
            break; // только первую строку читаем здесь
        }
    }

    std::ofstream out(outputFilename);
    if (!out.is_open()) {
        std::cerr << "Не удалось открыть выходной файл: " << outputFilename << "\n";
        return false;
    }

    size_t writeCount = 0;

    while (!minHeap.empty()) {
        Node current = minHeap.top();
        minHeap.pop();

        out << current.value << "\n";
        ++writeCount;

        if (writeCount % 1'000'000 == 0)
            LOG("Прогресс: записано " << writeCount << " строк");

        size_t idx = current.chunkIndex;
        if (!validChunk[idx]) continue;

        std::string line;
        if (std::getline(inputs[idx], line)) {
            if (!line.empty()) {
                std::istringstream iss(line);
                T value;

                if constexpr (is_number<T>) {
                    iss >> value;
                    if (iss.fail()) {
                        std::cerr << "Ошибка парсинга в чанке " << idx << ": " << line << "\n";
                    } else {
                        minHeap.push({value, idx});
                    }
                } else {
                    value = line;
                    minHeap.push({value, idx});
                }
            }
        } else {
        // EOF — чанк прочитан до конца
            inputs[idx].close();
            if (std::remove(partFilenames[idx].c_str()) != 0) {
                std::cerr << "⚠️ Не удалось удалить " << partFilenames[idx] << "\n";
            } else {
                LOG("Удалён: " << partFilenames[idx]);
            }
        }
    }

    LOG("Слияние завершено: " << outputFilename << " (" << writeCount << " строк)");


    return true;
}



#include <string>
#include <filesystem>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <list>
#include <vector>    
#include <string>
#include <iostream>





namespace fs = std::filesystem;

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
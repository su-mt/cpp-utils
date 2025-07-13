#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <type_traits>

template<typename T, typename SortFn>
void process(const std::string& inputFile, const std::string& outputFile, SortFn sorter) {
    std::ifstream in(inputFile);
    if (!in) {
        std::cerr << "Error opening " << inputFile << "\n";
        return;
    }

    std::vector<T> data;
    if constexpr (std::is_same_v<T, std::string>) {
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty())
                data.push_back(line);
        }
    } else {
        T x;
        while (in >> x) {
            data.push_back(x);
        }
    }
    in.close();

    sorter(data.begin(), data.end());

    std::ofstream out(outputFile);
    if constexpr (std::is_floating_point_v<T>) {
        out << std::fixed << std::setprecision(std::numeric_limits<T>::max_digits10);
    }
    for (const auto& v : data)
        out << v << '\n';
}

void printUsage(const char* name) {
    std::cerr << "Usage: " << name << " [-i|-f|-s] [--stable] <input_file> <output_file>\n";
    std::cerr << "  -i: int, -f: float, -s: string\n";
    std::cerr << "  --stable: use std::stable_sort (optional)\n";
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printUsage(argv[0]);
        return 1;
    }

    std::string type = argv[1];
    bool stable = false;
    std::string inputFile, outputFile;

    if (argc == 5 && std::string(argv[2]) == "--stable") {
        stable = true;
        inputFile = argv[3];
        outputFile = argv[4];
    } else if (argc == 4) {
        inputFile = argv[2];
        outputFile = argv[3];
    } else {
        printUsage(argv[0]);
        return 1;
    }

    if (type == "-i") {
        if (stable) {
            process<int>(inputFile, outputFile, [](auto begin, auto end) {
                std::stable_sort(begin, end);
            });
        } else {
            process<int>(inputFile, outputFile, [](auto begin, auto end) {
                std::sort(begin, end);
            });
        }
    } else if (type == "-f") {
        if (stable) {
            process<float>(inputFile, outputFile, [](auto begin, auto end) {
                std::stable_sort(begin, end);
            });
        } else {
            process<float>(inputFile, outputFile, [](auto begin, auto end) {
                std::sort(begin, end);
            });
        }
    } else if (type == "-s") {
        if (stable) {
            process<std::string>(inputFile, outputFile, [](auto begin, auto end) {
                std::stable_sort(begin, end);
            });
        } else {
            process<std::string>(inputFile, outputFile, [](auto begin, auto end) {
                std::sort(begin, end);
            });
        }
    } else {
        std::cerr << "Unknown type: " << type << "\n";
        return 1;
    }

    std::cout << "Sorting completed.\n";
    return 0;
}

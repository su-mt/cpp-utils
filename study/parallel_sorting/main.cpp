#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>

#include "mergeSorting.hpp"
#include "quickSort.hpp"



using namespace std;

const unsigned max_threads = 11;

void printUsage(const char* progName) {
    cerr << "Usage: " << progName
         << " [ -q | --quick | -m | --merge ]"
         << " [ -a | --ascending | -d | --descending ]"
         << " [ -i | --int | -f | --float | -s | --string ]"
         << " <input_file> <output_file>" << endl;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printUsage(argv[0]);
        return 1;
    }

    string mode = argv[1];
    string order = "ascending";
    string type = "string";
    vector<string> flags;
    for (int i = 2; i < argc - 2; ++i) flags.push_back(argv[i]);
    string inputFile = argv[argc - 2];
    string outputFile = argv[argc - 1];

    // Обработка флагов
    for (auto& flag : flags) {
        if (flag == "-a" || flag == "--ascending") order = "ascending";
        else if (flag == "-d" || flag == "--descending") order = "descending";
        else if (flag == "-i" || flag == "--int") type = "int";
        else if (flag == "-f" || flag == "--float") type = "float";
        else if (flag == "-s" || flag == "--string") type = "string";
        else {
            cerr << "Error: Unknown flag '" << flag << "'" << endl;
            printUsage(argv[0]);
            return 1;
        }
    }

    if (!filesystem::exists(inputFile)) {
        cerr << "Error: Input file does not exist: " << inputFile << endl;
        return 1;
    }

    // Быстрая сортировка
    if (mode == "-q" || mode == "--quick") {
        if (type == "int") {
            vector<int> data;
            ifstream in(inputFile);
            int v;
            while (in >> v) data.push_back(v);
            if (order == "ascending") quickSort<int>(data, 0, data.size() - 1);
            else quickSort<int>(data, 0, data.size() - 1, greater<int>());
            ofstream out(outputFile);
            for (auto x : data) out << x << '\n';
        } else if (type == "float") {
            vector<float> data;
            ifstream in(inputFile);
            float v;
            while (in >> v) data.push_back(v);
            if (order == "ascending") quickSort<float>(data, 0, data.size() - 1);
            else quickSort<float>(data, 0, data.size() - 1, greater<float>());
            ofstream out(outputFile);
            for (auto x : data) out << x << '\n';
        } else {
            vector<string> data;
            ifstream in(inputFile);
            string s;
            while (getline(in, s)) data.push_back(s);
            if (order == "ascending") quickSort<string>(data, 0, data.size() - 1);
            else quickSort<string>(data, 0, data.size() - 1, greater<string>());
            ofstream out(outputFile);
            for (auto& x : data) out << x << '\n';
        }
        cout << "Quick sort completed." << endl;
        return 0;
    }

    // Внешняя сортировка
    if (mode == "-m" || mode == "--merge") {
        if (type == "int") {
            int chunks = splitIntoChunks<int>(inputFile);
            if (order == "ascending") {
                sortAllChunks<int>(inputFile, chunks);
                mergeChunksToFile<int>(inputFile, chunks, outputFile);
            } else {
                sortAllChunks<int>(inputFile, chunks, greater<int>());
                mergeChunksToFile<int>(inputFile, chunks, outputFile, greater<int>());
            }
        } else if (type == "float") {
            int chunks = splitIntoChunks<float>(inputFile);
            if (order == "ascending") {
                sortAllChunks<float>(inputFile, chunks);
                mergeChunksToFile<float>(inputFile, chunks, outputFile);
            } else {
                sortAllChunks<float>(inputFile, chunks, greater<float>());
                mergeChunksToFile<float>(inputFile, chunks, outputFile, greater<float>());
            }
        } else {
            int chunks = splitIntoChunks(inputFile);
            if (order == "ascending") {
                sortAllChunks<string>(inputFile, chunks);
                mergeChunksToFile<string>(inputFile, chunks, outputFile);
            } else {
                sortAllChunks<string>(inputFile, chunks, greater<string>());
                mergeChunksToFile<string>(inputFile, chunks, outputFile, greater<string>());
            }
        }
        cout << "External merge sort completed." << endl;
        return 0;
    }

    cerr << "Error: Unknown mode '" << mode << "'" << endl;
    printUsage(argv[0]);
    return 1;
}

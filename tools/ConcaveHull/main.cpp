#include "ConcaveHull.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <cstring>

using namespace std;

int main(int argc, char** argv)
{
    bool forceOverwrite = false;
    double gamma = 1500.0;
    const char* inputPath = nullptr;
    const char* outputPath = nullptr;

    // Simple argument parsing
    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--force") == 0)) {
            forceOverwrite = true;
        } else if ((strcmp(argv[i], "-g") == 0) || (strcmp(argv[i], "--gamma") == 0)) {
            if (i + 1 < argc) {
                gamma = atof(argv[++i]);
            } else {
                cerr << "Error: Missing value after " << argv[i] << endl;
                return 1;
            }
        } else if (!inputPath) {
            inputPath = argv[i];
        } else if (!outputPath) {
            outputPath = argv[i];
        } else {
            cerr << "Unknown argument: " << argv[i] << endl;
            return 1;
        }
    }

    if (!inputPath || !outputPath) {
        cerr << "Usage: " << argv[0] << " [options] <input_file> <output_file>\n"
             << "Options:\n"
             << "  -f, --force            Overwrite output file without warning\n"
             << "  -g, --gamma <value>    Set gamma value for ConcaveHull (default: 1500.0)\n";
        return 1;
    }

    ifstream data(inputPath);
    if (!data) {
        cerr << "Input file: open error\n";
        return 1;
    }

    if (filesystem::exists(outputPath) && !forceOverwrite) {
        cout << "Warning: file \"" << outputPath << "\" already exists and will be overwritten!\n";
        cout << "Use -f or --force to skip this warning.\n";
        constexpr int wait_seconds = 5;
        cout << "Continuing in " << wait_seconds << " seconds...\n";
        this_thread::sleep_for(chrono::seconds(wait_seconds));
    }

    ofstream out(outputPath, ios::binary);
    if (!out) {
        cerr << "Output file: create error\n";
        return 1;
    }

    vector<Point> points;
    float x, y;
    while (data >> x >> y) {
        points.push_back({x, y});
    }

    ConcaveHull CH(points);
    list<Point> cnh = CH.getConcaveHull(gamma);

    for (const auto& pt : cnh) {
        out << pt.x << ' ' << pt.y << '\n';
    }
    auto pt = cnh.front();
    out << pt.x << ' ' << pt.y << '\n';

    return 0;
}

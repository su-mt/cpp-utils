// O(N) time complexity
// O(M) space complexity, where M is the number of unique lines
// Removes duplicate lines (triples of floats) from input file and writes only unique lines to output file.

// mt

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <functional>
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

class DeleteRepeatsData {
private:
    static constexpr size_t maxBufferSize = 8192;

    struct Triple {
        float a, b, c;

        bool operator==(const Triple& other) const {
            return a == other.a && b == other.b && c == other.c;
        }
    };

    struct TripleHash {
        size_t operator()(const Triple& t) const {
            hash<float> hf;
            size_t h1 = hf(t.a);
            size_t h2 = hf(t.b);
            size_t h3 = hf(t.c);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    unordered_set<Triple, TripleHash> seen_;
    char buffer_[maxBufferSize];
    size_t bufSize_ = 0;

public:
    int deleteRepeatsDats(ifstream& in, ofstream& out) {
        string line;
        int count = 0;

        while (getline(in, line)) {
            istringstream iss(line);
            float a, b, c;
            iss >> a >> b >> c;
            Triple t{a, b, c};

            if (seen_.find(t) == seen_.end()) {
                seen_.insert(t);
                ++count;

                line += '\n';
                const char* data = line.c_str();
                size_t dataLen = line.size();

                if (bufSize_ + dataLen > maxBufferSize) {
                    out.write(buffer_, bufSize_);
                    bufSize_ = 0;
                    continue;
                }

                memcpy(buffer_ + bufSize_, data, dataLen);
                bufSize_ += dataLen;
            }
        }

        if (bufSize_) {
            out.write(buffer_, bufSize_);
        }

        return count;
    }
};

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " </input_file/path> </output_file/path>" << endl;
        return 1;
    }

    ifstream data(argv[1]);
    if (!data) {
        cerr << "Input file: open error" << endl;
        return 1;
    }

    constexpr int wait_seconds = 5;
    if (filesystem::exists(argv[2])) {
        cout << "Warning: file \"" << argv[2] << "\" already exists and will be overwritten!" << endl;
        cout << "The program will continue in " << wait_seconds << " seconds..." << endl;
        this_thread::sleep_for(std::chrono::seconds(wait_seconds));

    }

    ofstream out(argv[2], ios::binary);
    if (!out) {
        cerr << "Output file: create error" << endl;
        return 1;
    }

    int uniqueLines = DeleteRepeatsData().deleteRepeatsDats(data, out);
    cout << "In file " << argv[2] << " successfully wrote " << uniqueLines << " unique lines" << endl;

    return 0;
}
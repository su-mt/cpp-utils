#include <string>

#include "sorting.hpp"

using namespace std;

const unsigned max_threads = 11;

int splitIntoChunks(const std::string& filename);



int main(int argc, char** argv) {
    ifstream file;
    if (argc != 3){
        cerr << "incorrect usage" << endl;
        return 1;
    }
    const string filename (argv[1]);

    int totalChunks = splitIntoChunks(filename);

    sortAllChunks<string>(filename, totalChunks);
    mergeChunksToFile<string>(filename, totalChunks, argv[3]);



    return 0;

}

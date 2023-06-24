#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <map>

using namespace std;
using namespace std::filesystem;

namespace file_reader {
    class FileReader {
        map<path, string> _files;

        public:
            FileReader(string);
            const map<path, string> get_files();
    };
};

#endif
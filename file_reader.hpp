#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

namespace file_reader {
    class FileReader {
        map<string, string> _files;

        public:
            FileReader(int, char*[]);
            const map<string, string> get_files();
            const string get_src();
    };
};

#endif
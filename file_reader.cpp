#include "file_reader.hpp"

namespace file_reader {
    FileReader::FileReader(int argc, char* argv[]) {
        for (int i = 1; i < argc; i++) {
            char ch;
            stringstream ss;
            ifstream fin(argv[i]);
            string text = "";

            while (fin >> noskipws >> ch) {
                text += ch;
            }

            _files[argv[i]] = text;
        }
    }

    const map<string, string> FileReader::get_files() {
        return this->_files;
    }

    const string FileReader::get_src() {
        string src = "";

        for (auto file : this->_files) {
            src += file.second;
        }

        return src;
    }
};

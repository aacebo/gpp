#include "file_reader.hpp"

namespace file_reader {
    FileReader::FileReader(string path) {
        for (recursive_directory_iterator i(path), end; i != end; i++) {
            if (!is_directory(i->path()) && i->path().extension().string() == ".gpp") {
                char ch;
                stringstream ss;
                ifstream fin(i->path());
                string text = "";

                while (fin >> noskipws >> ch) {
                    text += ch;
                }

                _files[i->path()] = text;
            }
        }
    }

    const map<path, string> FileReader::get_files() {
        return this->_files;
    }
};

#include <fstream>
#include <filesystem>
#include <sstream>

#include "lexer.hpp"

void lexer::read() {
    for (recursive_directory_iterator i("."), end; i != end; i++) {
        if (!is_directory(i->path()) && i->path().extension().string() == ".gpp") {
            char ch;
            stringstream ss;
            ifstream fin(i->path());
            string text = "";

            while (fin >> noskipws >> ch) {
                if (ch == '\n') {
                    text += ' ';
                } else {
                    text += ch;
                }
            }

            this->files[i->path()] = text;
        }
    }
}
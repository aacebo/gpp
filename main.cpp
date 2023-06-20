#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <map>

#include "scanner.hpp"

using namespace std;
using namespace std::filesystem;

int main() {
    map<path, scanner*> files;

    for (recursive_directory_iterator i("."), end; i != end; i++) {
        if (!is_directory(i->path()) && i->path().extension().string() == ".gpp") {
            char ch;
            stringstream ss;
            ifstream fin(i->path());
            string text = "";

            while (fin >> noskipws >> ch) {
                text += ch;
            }

            auto s = new scanner();
            s->scan(text);
            files[i->path()] = s;

            for (auto t : s->get_tokens()) {
                cout << t->get_type() << " "
                     << t->get_lex() << " "
                     << t->get_ln() << endl;
            }
        }
    }

    for (auto s : files) {
        delete s.second;
    }

    return 0;
}

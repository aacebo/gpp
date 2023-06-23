#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <map>

#include "scanner.hpp"
#include "expression.hpp"

using namespace std;
using namespace std::filesystem;

int main() {
    map<path, scanner::Scanner*> files;

    for (recursive_directory_iterator i("."), end; i != end; i++) {
        if (!is_directory(i->path()) && i->path().extension().string() == ".gpp") {
            char ch;
            stringstream ss;
            ifstream fin(i->path());
            string text = "";

            while (fin >> noskipws >> ch) {
                text += ch;
            }

            auto s = new scanner::Scanner(text);
            files[i->path()] = s;

            for (auto t : s->get_tokens()) {
                cout << t.to_string() << endl;
            }
        }
    }

    for (auto s : files) {
        delete s.second;
    }

    files.clear();
    return 0;
}

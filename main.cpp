#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

using namespace std;
using namespace std::filesystem;

int main() {
    string text = "";

    for (recursive_directory_iterator i("."), end; i != end; i++) {
        if (!is_directory(i->path()) && i->path().extension().string() == ".gpp") {
            char ch;
            stringstream ss;
            ifstream fin(i->path());

            while (fin >> noskipws >> ch) {
                if (ch == '\n') {
                    text += ' ';
                } else {
                    text += ch;
                }
            }
        }
    }

    cout << text << endl;

    return 0;
}

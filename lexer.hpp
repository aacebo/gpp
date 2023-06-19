#pragma once

#include <map>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

class lexer {
    public:
        map<path, string> files;
        void read();
};

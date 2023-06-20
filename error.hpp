#pragma once

#include <exception>
#include <string>

using namespace std;

class error : public exception {
    int _ln_;
    int _col_;
    const char* _msg_;

    public:
        error(int ln, int col, string msg) : _ln_(ln), _col_(col), _msg_(msg.c_str()) { }

        const char* what() {
            return ("[ln: " + to_string(_ln_) + ", col: " + to_string(_col_) + "] - " + _msg_).c_str();
        }
};

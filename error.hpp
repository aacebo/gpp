#pragma once

#include <exception>
#include <string>

using namespace std;

class error : exception {
    int _ln_;
    int _col_;
    const string _msg_;

    public:
        error(int ln, int col, string msg) : _ln_(ln), _col_(col), _msg_(msg) { }

        const string what() {
            return "[ln: " + to_string(_ln_) + ", col: " + to_string(_col_) + "] - " + _msg_;
        }
};

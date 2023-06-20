#pragma once

#include <string>
#include <vector>

#include "token.hpp"
#include "error.hpp"

using namespace std;

class scanner {
    string _src_;
    int _left_, _right_, _ln_;
    vector<token*> _tokens_;
    vector<error*> _errors_;

    public:
        scanner(string);
        ~scanner();

        const vector<token*> get_tokens() { return this->_tokens_; }
        const vector<error*> get_errors() { return this->_errors_; }

    private:
        void push(token::type);
        char peek();
        bool is_integer(char c);
        bool is_alpha(char c);
        void on_comment();
        void on_string();
        void on_number();
        void on_identifier();
};

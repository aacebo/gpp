#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>

#include "token.hpp"
#include "error.hpp"

using namespace std;

namespace scanner {
    class Scanner {
        const string _src;
        int _left;
        int _right;
        int _ln;
        vector<token::Token*> _tokens;
        vector<error::Error*> _errors;

        public:
            Scanner(string);
            const vector<token::Token*> get_tokens();
            const vector<error::Error*> get_errors();

        private:
            void scan();
            void push(token::Type);
            char peek();
            bool is_integer(char);
            bool is_alpha(char);
            void on_comment();
            void on_string();
            void on_number();
            void on_identifier();
    };
};

#endif
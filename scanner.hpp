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

        public:
            Scanner(string);
            token::Token scan();

        private:
            token::Token create(token::Type);
            char peek();
            bool is_escaped();
            bool is_integer(char);
            bool is_alpha(char);
            token::Token on_comment();
            token::Token on_string();
            token::Token on_number();
            token::Token on_identifier();
    };
};

#endif
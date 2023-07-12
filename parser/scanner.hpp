#ifndef SCANNER_H
#define SCANNER_H

#include <string>

#include "../error.hpp"

#include "token.hpp"
#include "token_type.hpp"

using namespace std;

namespace parser {
    class Scanner {
        const string _src;
        int _left;
        int _right;
        int _ln;

        public:
            Scanner(const string&);
            Token* scan();

        private:
            Token* create(Type);
            char peek();
            bool is_escaped();
            bool is_integer(char);
            bool is_alpha(char);
            Token* on_comment();
            Token* on_string();
            Token* on_number();
            Token* on_identifier();
    };
};

#endif
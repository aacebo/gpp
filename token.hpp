#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>

#include "token_type.hpp"
#include "error.hpp"

using namespace std;

namespace scanner {
    class Token {
        public:
            const Type type;
            const int ln;
            const int start;
            const int end;
            const string value;

            Token(Type, int, int, int, string&);
            Token(const Token&);

            const string to_string();
            const float to_float();
            const bool to_bool();
    };
};

#endif
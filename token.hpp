#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>

#include "token_type.hpp"

using namespace std;

namespace token {
    class Token {
        public:
            const Type type;
            const int ln;
            const int start;
            const int end;
            const string value;

            Token(Type, int, int, int, string);
            const string fmt();
            const float to_float();
            const bool to_bool();
    };
};

#endif
#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "token.hpp"
#include "chunk.hpp"

using namespace std;

namespace parser {
    enum class Precedence {
        None,
        Assignment,
        Or,
        And,
        Equality,
        Comparison,
        Term,
        Factor,
        Unary,
        Call,
        Primary
    };

    class Parser {
        public:
            const vector<token::Token> tokens;
            vector<Chunk> chunks;

            Parser(vector<token::Token>);
    };
};

#endif
#pragma once

#include <vector>

#include "token.hpp"

using namespace std;

namespace parser {
    class Parser {
        const vector<token::Token> _tokens;

        public:
            Parser(vector<token::Token>);
    };
};

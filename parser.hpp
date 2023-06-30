#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "scanner.hpp"
#include "chunk.hpp"
#include "error.hpp"

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
        scanner::Scanner* scanner;

        public:
            scanner::Token* curr;
            scanner::Token* prev;
            vector<error::Error> errors;

            Parser(string);
            ~Parser();

            bool next();
            bool match(scanner::Type);
            void consume(scanner::Type, string);
    };
};

#endif
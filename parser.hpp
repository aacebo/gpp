#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "scanner.hpp"
#include "error.hpp"
#include "value.hpp"

using namespace std;

namespace parser {
    enum class Precedence {
        None,
        Assignment, // =
        Or,         // ||
        And,        // &&
        Equality,   // ==, !=
        Comparison, // <, <=, >, >=
        Term,       // +, +=, -, -=
        Factor,     // *, *=, /, /=
        Unary,      // !, -
        Call,       // ., ()
        Primary
    };

    typedef void (*ParseFn)(bool canAssign);

    class Rule {
        const ParseFn prefix;
        const ParseFn infix;
        const Precedence precedence;

        public:
            Rule(ParseFn, ParseFn, Precedence);
    };

    class Parser {
        scanner::Scanner* scanner;
        vector<error::Error> errors;

        public:
            scanner::Token* curr;
            scanner::Token* prev;

            Parser(const string&);
            ~Parser();

            bool next();
            bool match(scanner::Type);
            void consume(scanner::Type, const string&);
            void sync();
            Rule get_token_rule(scanner::Type);
    };
};

#endif
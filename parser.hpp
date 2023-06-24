#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "token.hpp"
#include "error.hpp"
#include "expression.hpp"
#include "statement.hpp"

using namespace std;

namespace parser {
    class Parser {
        const vector<token::Token> _tokens;
        vector<error::Error> _errors;
        int _it = 0;

        public:
            Parser(vector<token::Token>);

        private:
            token::Token next();
            token::Token prev();
            token::Token peek();
            const bool is_end();
            const bool is_type(token::Type);
            const error::SyntaxError error(token::Token, string);
            const bool match(initializer_list<token::Type>);
            const token::Token consume(token::Type, string);
            const expression::Expression _assignment();
            const expression::Expression _expression();
            const expression::Expression _primary();
            const expression::Expression _or();
            const expression::Expression _and();
            const expression::Expression _equality();
            const expression::Expression _comparison();
            const expression::Expression _term();
            const expression::Expression _factor();
            const expression::Expression _unary();
            const expression::Expression _call();
            const expression::Expression _call_finish(expression::Expression);
    };
};

#endif
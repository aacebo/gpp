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
        const vector<token::Token*> _tokens;
        vector<statement::Statement*> _statements;
        vector<error::Error*> _errors;
        int _it = 0;

        public:
            Parser(vector<token::Token*>);
            ~Parser();
            const vector<statement::Statement*> get_statements();
            const vector<error::Error*> get_errors();

        private:
            token::Token* next();
            token::Token* prev();
            token::Token* peek();
            token::Token* consume(token::Type, string);
            error::SyntaxError* error(token::Token*, string);

            bool is_end();
            bool is_type(token::Type);
            bool match(initializer_list<token::Type>);
            void sync();

            // expressions
            expression::Expression* _assignment();
            expression::Expression* _expression();
            expression::Expression* _primary();
            expression::Expression* _or();
            expression::Expression* _and();
            expression::Expression* _equality();
            expression::Expression* _comparison();
            expression::Expression* _term();
            expression::Expression* _factor();
            expression::Expression* _unary();
            expression::Expression* _call();
            expression::Expression* _call_finish(expression::Expression*);

            // statements
            statement::Statement* _declaration();
            statement::Statement* _statement();
            statement::Statement* _class();
            statement::Statement* _for();
            statement::Statement* _if();
            statement::Statement* _print();
            statement::Statement* _return();
            statement::Statement* _var();
            statement::Statement* _expr();
            statement::Statement* _use();
            statement::Function* _function(string);
            vector<statement::Statement*> _block();
    };
};

#endif
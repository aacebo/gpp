#pragma once

#include <vector>
#include <string>

#include "expression.hpp"
#include "token.hpp"
#include "statement_type.hpp"

using namespace std;

namespace statement {
    class Statement {
        public:
            Type type = Type::Default;

            virtual string to_string() { return type_to_string(this->type); }
    };

    class Block : public Statement {
        public:
            const Type type = Type::Block;
            vector<Statement*> stmts;

            Block(vector<Statement*> stmts) : stmts(stmts) { }
    };

    class Function : public Statement {
        public:
            const Type type = Type::Function;
            token::Token* name;
            vector<token::Token*> params;
            vector<Statement*> body;

            Function(token::Token* name, vector<token::Token*> params, vector<Statement*> body) : name(name), params(params), body(body) { }
    };

    class Class : public Statement {
        public:
            const Type type = Type::Class;
            token::Token* name;
            expression::Variable* superclass;
            vector<Function*> methods;

            Class(token::Token* name, expression::Variable* superclass, vector<Function*> methods) : name(name), superclass(superclass), methods(methods) { }
    };

    class Expression : public Statement {
        public:
            const Type type = Type::Expression;
            expression::Expression* exp;

            Expression(expression::Expression* exp) : exp(exp) { }
    };

    class If : public Statement {
        public:
            const Type type = Type::If;
            expression::Expression* condition;
            Statement* then_branch;
            Statement* else_branch;

            If(expression::Expression* condition, Statement* then_branch, Statement* else_branch) : condition(condition), then_branch(then_branch), else_branch(else_branch) { }
    };

    class Return : public Statement {
        public:
            const Type type = Type::Return;
            token::Token* keyword;
            expression::Expression* value;

            Return(token::Token* keyword, expression::Expression* value) : keyword(keyword), value(value) { }
    };

    class Let : public Statement {
        public:
            const Type type = Type::Let;
            token::Token* name;
            expression::Expression* init;

            Let(token::Token* name, expression::Expression* init) : name(name), init(init) { }
    };

    class For : public Statement {
        public:
            const Type type = Type::For;
            expression::Expression* condition;
            Statement* body;

            For(expression::Expression* condition, Statement* body) : condition(condition), body(body) { }
    };

    class Use : public Statement {
        public:
            const Type type = Type::Use;
            token::Token* name;

            Use(token::Token* name) : name(name) { }
    };
};

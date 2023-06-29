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
            Type type;

            virtual string to_string() { return type_to_string(this->type); }
    };

    class Block : public Statement {
        public:
            vector<Statement*> stmts;

            Block(vector<Statement*> stmts) : stmts(stmts) {
                this->type = Type::Block;
            }
    };

    class Function : public Statement {
        public:
            token::Token* name;
            token::Token* return_type;
            token::Token* optional;
            vector<token::Token*> params;
            vector<token::Token*> param_types;
            vector<Statement*> body;

            Function(
                token::Token* name,
                token::Token* return_type,
                token::Token* optional,
                vector<token::Token*> params,
                vector<token::Token*> param_types,
                vector<Statement*> body
            ) : name(name), return_type(return_type), optional(optional), params(params), param_types(param_types), body(body) {
                this->type = Type::Function;
            }
    };

    class Class : public Statement {
        public:
            token::Token* name;
            expression::Variable* super;
            vector<Function*> methods;

            Class(token::Token* name, expression::Variable* super, vector<Function*> methods) : name(name), super(super), methods(methods) {
                this->type = Type::Class;
            }
    };

    class Expression : public Statement {
        public:
            expression::Expression* exp;

            Expression(expression::Expression* exp) : exp(exp) {
                this->type = Type::Expression;
            }
    };

    class If : public Statement {
        public:
            expression::Expression* condition;
            Statement* then_branch;
            Statement* else_branch;

            If(expression::Expression* condition, Statement* then_branch, Statement* else_branch) : condition(condition), then_branch(then_branch), else_branch(else_branch) {
                this->type = Type::If;
            }
    };

    class Print : public Statement {
        public:
            expression::Expression* expr;

            Print(expression::Expression* expr) : expr(expr) {
                this->type = Type::Print;
            }
    };

    class Return : public Statement {
        public:
            token::Token* keyword;
            expression::Expression* value;

            Return(token::Token* keyword, expression::Expression* value) : keyword(keyword), value(value) {
                this->type = Type::Return;
            }
    };

    class Let : public Statement {
        public:
            token::Token* keyword;
            token::Token* name;
            token::Token* _type;
            token::Token* optional;
            expression::Expression* init;

            Let(
                token::Token* keyword,
                token::Token* name,
                token::Token* _type,
                token::Token* optional,
                expression::Expression* init
            ) : keyword(keyword), name(name), _type(_type), optional(optional), init(init) {
                this->type = Type::Let;
            }
    };

    class For : public Statement {
        public:
            expression::Expression* condition;
            Statement* body;

            For(expression::Expression* condition, Statement* body) : condition(condition), body(body) {
                this->type = Type::For;
            }
    };

    class Use : public Statement {
        public:
            token::Token* name;

            Use(token::Token* name) : name(name) {
                this->type = Type::Use;
            }
    };
};

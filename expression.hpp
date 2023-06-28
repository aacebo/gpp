#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <any>
#include <vector>
#include <string>

#include "token.hpp"
#include "expression_type.hpp"

using namespace std;

namespace expression {
    class Expression {
        public:
            Type type;

            virtual string to_string() { return type_to_string(this->type); }
    };

    class Assign : public Expression {
        public:
            token::Token* name;
            Expression* value;

            Assign(token::Token* name, Expression* value) : name(name), value(value) {
                this->type = Type::Assign;
            }
    };

    class Binary : public Expression {
        public:
            Expression* left;
            token::Token* op;
            Expression* right;

            Binary(Expression* left, token::Token* op, Expression* right) : left(left), op(op), right(right) {
                this->type = Type::Binary;
            }
    };

    class Call : public Expression {
        public:
            Expression* name;
            token::Token* paren;
            vector<Expression*> args;

            Call(Expression* name, token::Token* paren, vector<Expression*> args) : name(name), paren(paren), args(args) {
                this->type = Type::Call;
            }
    };

    class Get : public Expression {
        public:
            Expression* object;
            token::Token* name;

            Get(Expression* object, token::Token* name) : object(object), name(name) {
                this->type = Type::Get;
            }
    };

    class Set : public Expression {
        public:
            Expression* object;
            token::Token* name;
            Expression* value;

            Set(Expression* object, token::Token* name, Expression* value) : object(object), name(name), value(value) {
                this->type = Type::Set;
            }
    };

    class Grouping : public Expression {
        public:
            Expression* exp;

            Grouping(Expression* exp) : exp(exp)  {
                this->type = Type::Grouping;
            }
    };

    class Literal : public Expression {
        public:
            any value;

            Literal() { this->type = Type::Literal; }
            Literal(any value) : value(value) { this->type = Type::Literal; }
            Literal(float value) : value(value) { this->type = Type::Literal; }
            Literal(bool value) : value(value) { this->type = Type::Literal; }
            Literal(string value) : value(value) { this->type = Type::Literal; }
    };

    class Logical : public Expression {
        public:
            Expression* left;
            token::Token* op;
            Expression* right;

            Logical(Expression* left, token::Token* op, Expression* right) : left(left), op(op), right(right) {
                this->type = Type::Logical;
            }
    };

    class Super : public Expression {
        public:
            token::Token* keyword;
            token::Token* method;

            Super(token::Token* keyword, token::Token* method) : keyword(keyword), method(method) {
                this->type = Type::Super;
            }
    };

    class Self : public Expression {
        public:
            token::Token* keyword;

            Self(token::Token* keyword) : keyword(keyword) {
                this->type = Type::Self;
            }
    };

    class Unary : public Expression {
        public:
            token::Token* op;
            Expression* right;

            Unary(token::Token* op, Expression* right) : op(op), right(right) {
                this->type = Type::Unary;
            }
    };

    class Variable : public Expression {
        public:
            token::Token* name;

            Variable(token::Token* name) : name(name) {
                this->type = Type::Variable;
            }
    };
}

#endif
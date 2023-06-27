#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include <string>

#include "token.hpp"
#include "expression_type.hpp"

using namespace std;

namespace expression {
    class Expression {
        public:
            Type type = Type::Default;

            virtual string to_string() { return type_to_string(this->type); }
    };

    class Assign : public Expression {
        public:
            const Type type = Type::Assign;
            token::Token* name;
            Expression* value;

            Assign(token::Token* name, Expression* value) : name(name), value(value) { }
    };

    class Binary : public Expression {
        public:
            const Type type = Type::Binary;
            Expression* left;
            token::Token* op;
            Expression* right;

            Binary(Expression* left, token::Token* op, Expression* right) : left(left), op(op), right(right)  { }
    };

    class Call : public Expression {
        public:
            const Type type = Type::Call;
            Expression* name;
            token::Token* paren;
            vector<Expression*> args;

            Call(Expression* name, token::Token* paren, vector<Expression*> args) : name(name), paren(paren), args(args) { }
    };

    class Get : public Expression {
        public:
            const Type type = Type::Get;
            Expression* object;
            token::Token* name;

            Get(Expression* object, token::Token* name) : object(object), name(name) { }
    };

    class Set : public Expression {
        public:
            const Type type = Type::Set;
            Expression* object;
            token::Token* name;
            Expression* value;

            Set(Expression* object, token::Token* name, Expression* value) : object(object), name(name), value(value) { }
    };

    class Grouping : public Expression {
        public:
            const Type type = Type::Grouping;
            Expression* exp;

            Grouping(Expression* exp) : exp(exp)  { }
    };

    template <class V>
    class Literal : public Expression {
        public:
            const Type type = Type::Literal;
            const V value;

            Literal(V value) : value(value)  { }
    };

    class Logical : public Expression {
        public:
            const Type type = Type::Logical;
            Expression* left;
            token::Token* op;
            Expression* right;

            Logical(Expression* left, token::Token* op, Expression* right) : left(left), op(op), right(right) { }
    };

    class Super : public Expression {
        public:
            const Type type = Type::Super;
            token::Token* keyword;
            token::Token* method;

            Super(token::Token* keyword, token::Token* method) : keyword(keyword), method(method) { }
    };

    class Self : public Expression {
        public:
            const Type type = Type::Self;
            token::Token* keyword;

            Self(token::Token* keyword) : keyword(keyword) { }
    };

    class Unary : public Expression {
        public:
            const Type type = Type::Unary;
            token::Token* op;
            Expression* right;

            Unary(token::Token* op, Expression* right) : op(op), right(right) { }
    };

    class Variable : public Expression {
        public:
            const Type type = Type::Variable;
            token::Token* name;

            Variable(token::Token* name) : name(name) { }
    };
}

#endif
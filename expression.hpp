#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>

#include "token.hpp"

using namespace std;

namespace expression {
    template <class> class Visitor;

    enum class Type {
        Assign,
        Binary,
        Call,
        Get,
        Set,
        Grouping,
        Literal,
        Logical,
        Super,
        Self,
        Unary,
        Variable
    };

    class Expression {
        public:
            Type type;

            template <class T>
            T accept(Visitor<T> v) { }
    };

    class Assign : public Expression {
        public:
            const Type type = Type::Assign;
            const token::Token name;
            const Expression value;

            Assign(token::Token name, Expression value) : name(name), value(value) { }

            template <class T>
            T accept(Visitor<T> v) { return v.assign(this); }
    };

    class Binary : public Expression {
        public:
            const Type type = Type::Binary;
            const Expression left;
            const token::Token op;
            const Expression right;

            Binary(Expression left, token::Token op, Expression right) : left(left), op(op), right(right)  { }

            template <class T>
            T accept(Visitor<T> v) { return v.binary(this); }
    };

    class Call : public Expression {
        public:
            const Type type = Type::Call;
            const Expression name;
            const token::Token paren;
            const vector<Expression> args;

            Call(Expression name, token::Token paren, vector<Expression> args) : name(name), paren(paren), args(args) { }

            template <class T>
            T accept(Visitor<T> v) { return v.call(this); }
    };

    class Get : public Expression {
        public:
            const Type type = Type::Get;
            const Expression object;
            const token::Token name;

            Get(Expression object, token::Token name) : object(object), name(name) { }

            template <class T>
            T accept(Visitor<T> v) { return v.get(this); }
    };

    class Set : public Expression {
        public:
            const Type type = Type::Set;
            const Expression object;
            const token::Token name;
            const Expression value;

            Set(Expression object, token::Token name, Expression value) : object(object), name(name), value(value) { }

            template <class T>
            T accept(Visitor<T> v) { return v.set(this); }
    };

    class Grouping : public Expression {
        public:
            const Type type = Type::Grouping;
            const Expression exp;

            Grouping(Expression exp) : exp(exp)  { }

            template <class T>
            T accept(Visitor<T> v) { return v.grouping(this); }
    };

    template <class V>
    class Literal : public Expression {
        public:
            const Type type = Type::Literal;
            const V value;

            Literal(V value) : value(value)  { }

            template <class T>
            T accept(Visitor<T> v) { return v.literal(this); }
    };

    class Logical : public Expression {
        public:
            const Type type = Type::Logical;
            const Expression left;
            const token::Token op;
            const Expression right;

            Logical(Expression left, token::Token o, Expression right) : left(left), op(o), right(right) { }

            template <class T>
            T accept(Visitor<T> v) { return v.logical(this); }
    };

    class Super : public Expression {
        public:
            const Type type = Type::Super;
            const token::Token keyword;
            const token::Token method;

            Super(token::Token keyword, token::Token method) : keyword(keyword), method(method) { }

            template <class T>
            T accept(Visitor<T> v) { return v.super(this); }
    };

    class Self : public Expression {
        public:
            const Type type = Type::Self;
            const token::Token keyword;

            Self(token::Token keyword) : keyword(keyword) { }

            template <class T>
            T accept(Visitor<T> v) { return v.self(this); }
    };

    class Unary : public Expression {
        public:
            const Type type = Type::Unary;
            const token::Token op;
            const Expression right;

            Unary(token::Token op, Expression right) : op(op), right(right) { }

            template <class T>
            T accept(Visitor<T> v) { return v.unary(this); }
    };

    class Variable : public Expression {
        public:
            const Type type = Type::Variable;
            const token::Token name;

            Variable(token::Token name) : name(name) { }

            template <class T>
            T accept(Visitor<T> v) { return v.variable(this); }
    };
}

#endif
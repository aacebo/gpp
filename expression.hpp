#pragma once

#include <vector>

#include "token.hpp"
#include "visitor.hpp"

using namespace std;

namespace expression {
    template <class> class Visitor;

    class Expression {
        public:
            template <class T>
            T accept(Visitor<T> v) {}
    };

    class Assign : Expression {
        public:
            const token::Token name;
            const Expression value;

            Assign(token::Token n, Expression v) : name(n), value(v) { }

            template <class T>
            T accept(Visitor<T> v) { return v.assign(this); }
    };

    class Binary : Expression {
        public:
            const Expression eleft;
            const token::Token op;
            const Expression eright;

            Binary(Expression left, token::Token o, Expression right) : eleft(left), op(o), eright(right)  { }

            template <class T>
            T accept(Visitor<T> v) { return v.binary(this); }
    };

    class Call : Expression {
        public:
            const Expression name;
            const token::Token paren;
            const vector<Expression> args;

            Call(Expression n, token::Token p, vector<Expression> a) : name(n), paren(p), args(a) { }

            template <class T>
            T accept(Visitor<T> v) { return v.call(this); }
    };

    class Get : Expression {
        public:
            const Expression object;
            const token::Token name;

            Get(Expression o, token::Token n) : object(o), name(n) { }

            template <class T>
            T accept(Visitor<T> v) { return v.get(this); }
    };

    class Set : Expression {
        public:
            const Expression object;
            const token::Token name;
            const Expression value;

            Set(Expression o, token::Token n, Expression v) : object(o), name(n), value(v) { }

            template <class T>
            T accept(Visitor<T> v) { return v.set(this); }
    };

    class Grouping : Expression {
        public:
            const Expression e;

            Grouping(Expression exp) : e(exp)  { }

            template <class T>
            T accept(Visitor<T> v) { return v.grouping(this); }
    };

    template <class V>
    class Literal : Expression {
        public:
            const V value;

            Literal(V v) : value(v)  { }

            template <class T>
            T accept(Visitor<T> v) { return v.literal(this); }
    };

    class Logical : Expression {
        public:
            const Expression eleft;
            const token::Token op;
            const Expression eright;

            Logical(Expression left, token::Token o, Expression right) : eleft(left), op(o), eright(right) { }

            template <class T>
            T accept(Visitor<T> v) { return v.logical(this); }
    };

    class Super : Expression {
        public:
            const token::Token keyword;
            const token::Token method;

            Super(token::Token k, token::Token m) : keyword(k), method(m) { }

            template <class T>
            T accept(Visitor<T> v) { return v.super(this); }
    };

    class Self : Expression {
        public:
            const token::Token keyword;

            Self(token::Token k) : keyword(k) { }

            template <class T>
            T accept(Visitor<T> v) { return v.self(this); }
    };

    class Unary : Expression {
        public:
            const token::Token op;
            const Expression eright;

            Unary(token::Token o, Expression right) : op(o), eright(right)  { }

            template <class T>
            T accept(Visitor<T> v) { return v.unary(this); }
    };

    class Let : Expression {
        public:
            const token::Token name;

            Let(token::Token n) : name(n) { }

            template <class T>
            T accept(Visitor<T> v) { return v.let(this); }
    };
}
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>

#include "token.hpp"

using namespace std;

namespace expression {
    template <class> class Visitor;

    class Expression {
        public:
            template <class T>
            T accept(Visitor<T>* v) { throw logic_error("unimplemented"); }
    };

    class Assign : public Expression {
        public:
            const token::Token name;
            const Expression value;

            Assign(token::Token name, Expression value) : name(name), value(value) { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_assign(this); }
    };

    class Binary : public Expression {
        public:
            const Expression left;
            const token::Token op;
            const Expression right;

            Binary(Expression left, token::Token op, Expression right) : left(left), op(op), right(right)  { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_binary(this); }
    };

    class Call : public Expression {
        public:
            const Expression name;
            const token::Token paren;
            const vector<Expression> args;

            Call(Expression name, token::Token paren, vector<Expression> args) : name(name), paren(paren), args(args) { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_call(this); }
    };

    class Get : public Expression {
        public:
            const Expression object;
            const token::Token name;

            Get(Expression object, token::Token name) : object(object), name(name) { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_get(this); }
    };

    class Set : public Expression {
        public:
            const Expression object;
            const token::Token name;
            const Expression value;

            Set(Expression object, token::Token name, Expression value) : object(object), name(name), value(value) { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_set(this); }
    };

    class Grouping : public Expression {
        public:
            const Expression exp;

            Grouping(Expression exp) : exp(exp)  { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_grouping(this); }
    };

    template <class V>
    class Literal : public Expression {
        public:
            const V value;

            Literal(V value) : value(value)  { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_literal(this); }
    };

    class Logical : public Expression {
        public:
            const Expression left;
            const token::Token op;
            const Expression right;

            Logical(Expression left, token::Token op, Expression right) : left(left), op(op), right(right) { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_logical(this); }
    };

    class Super : public Expression {
        public:
            const token::Token keyword;
            const token::Token method;

            Super(token::Token keyword, token::Token method) : keyword(keyword), method(method) { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_super(this); }
    };

    class Self : public Expression {
        public:
            const token::Token keyword;

            Self(token::Token keyword) : keyword(keyword) { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_self(this); }
    };

    class Unary : public Expression {
        public:
            const token::Token op;
            const Expression right;

            Unary(token::Token op, Expression right) : op(op), right(right) { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_unary(this); }
    };

    class Variable : public Expression {
        public:
            const token::Token name;

            Variable(token::Token name) : name(name) { }

            template <class T>
            T accept(Visitor<T>* v) { return v->visit_variable(this); }
    };
}

#endif
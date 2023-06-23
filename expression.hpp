#pragma once

#include <vector>

#include "token.hpp"
#include "visitor.hpp"

using namespace std;

namespace expression {
    template <class> class visitor;

    class expression {
        public:
            template <class T>
            T accept(visitor<T> v) {}
    };

    class assign : expression {
        public:
            const token::Token name;
            const expression value;

            assign(token::Token n, expression v) : name(n), value(v) { }

            template <class T>
            T accept(visitor<T> v) { return v.assign(this); }
    };

    class binary : expression {
        public:
            const expression eleft;
            const token::Token op;
            const expression eright;

            binary(expression left, token::Token o, expression right) : eleft(left), op(o), eright(right)  { }

            template <class T>
            T accept(visitor<T> v) { return v.binary(this); }
    };

    class call : expression {
        public:
            const expression name;
            const token::Token paren;
            const vector<expression> args;

            call(expression n, token::Token p, vector<expression> a) : name(n), paren(p), args(a) { }

            template <class T>
            T accept(visitor<T> v) { return v.call(this); }
    };

    class get : expression {
        public:
            const expression object;
            const token::Token name;

            get(expression o, token::Token n) : object(o), name(n) { }

            template <class T>
            T accept(visitor<T> v) { return v.get(this); }
    };

    class set : expression {
        public:
            const expression object;
            const token::Token name;
            const expression value;

            set(expression o, token::Token n, expression v) : object(o), name(n), value(v) { }

            template <class T>
            T accept(visitor<T> v) { return v.set(this); }
    };

    class grouping : expression {
        public:
            const expression e;

            grouping(expression exp) : e(exp)  { }

            template <class T>
            T accept(visitor<T> v) { return v.grouping(this); }
    };

    template <class V>
    class literal : expression {
        public:
            const V value;

            literal(V v) : value(v)  { }

            template <class T>
            T accept(visitor<T> v) { return v.literal(this); }
    };

    class logical : expression {
        public:
            const expression eleft;
            const token::Token op;
            const expression eright;

            logical(expression left, token::Token o, expression right) : eleft(left), op(o), eright(right) { }

            template <class T>
            T accept(visitor<T> v) { return v.logical(this); }
    };

    class super : expression {
        public:
            const token::Token keyword;
            const token::Token method;

            super(token::Token k, token::Token m) : keyword(k), method(m) { }

            template <class T>
            T accept(visitor<T> v) { return v.super(this); }
    };

    class self : expression {
        public:
            const token::Token keyword;

            self(token::Token k) : keyword(k) { }

            template <class T>
            T accept(visitor<T> v) { return v.self(this); }
    };

    class unary : expression {
        public:
            const token::Token op;
            const expression eright;

            unary(token::Token o, expression right) : op(o), eright(right)  { }

            template <class T>
            T accept(visitor<T> v) { return v.unary(this); }
    };

    class let : expression {
        public:
            const token::Token name;

            let(token::Token n) : name(n) { }

            template <class T>
            T accept(visitor<T> v) { return v.let(this); }
    };
}
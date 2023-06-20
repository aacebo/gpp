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
            const token name;
            const expression value;

            assign(token n, expression v) : name(n), value(v) { }

            template <class T>
            T accept(visitor<T> v) { return v.assign(this); }
    };

    class binary : expression {
        public:
            const expression eleft;
            const token op;
            const expression eright;

            binary(expression left, token o, expression right) : eleft(left), op(o), eright(right)  { }

            template <class T>
            T accept(visitor<T> v) { return v.binary(this); }
    };

    class call : expression {
        public:
            const expression name;
            const token paren;
            const vector<expression> args;

            call(expression n, token p, vector<expression> a) : name(n), paren(p), args(a) { }

            template <class T>
            T accept(visitor<T> v) { return v.call(this); }
    };

    class get : expression {
        public:
            const expression object;
            const token name;

            get(expression o, token n) : object(o), name(n) { }

            template <class T>
            T accept(visitor<T> v) { return v.get(this); }
    };

    class set : expression {
        public:
            const expression object;
            const token name;
            const expression value;

            set(expression o, token n, expression v) : object(o), name(n), value(v) { }

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
            const token op;
            const expression eright;

            logical(expression left, token o, expression right) : eleft(left), op(o), eright(right) { }

            template <class T>
            T accept(visitor<T> v) { return v.logical(this); }
    };

    class super : expression {
        public:
            const token keyword;
            const token method;

            super(token k, token m) : keyword(k), method(m) { }

            template <class T>
            T accept(visitor<T> v) { return v.super(this); }
    };

    class self : expression {
        public:
            const token keyword;

            self(token k) : keyword(k) { }

            template <class T>
            T accept(visitor<T> v) { return v.self(this); }
    };

    class unary : expression {
        public:
            const token op;
            const expression eright;

            unary(token o, expression right) : op(o), eright(right)  { }

            template <class T>
            T accept(visitor<T> v) { return v.unary(this); }
    };

    class let : expression {
        public:
            const token name;

            let(token n) : name(n) { }

            template <class T>
            T accept(visitor<T> v) { return v.let(this); }
    };
}
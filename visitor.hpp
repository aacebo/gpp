#pragma once

#include "expression.hpp"

namespace expression {
    class assign;
    class binary;
    class call;
    class get;
    class set;
    class grouping;
    template <class> class literal;
    class logical;
    class super;
    class self;
    class unary;
    class let;

    template <class T>
    class visitor {
        public:
            virtual T assign(assign* e) = 0;
            virtual T binary(binary* e) = 0;
            virtual T call(call* e) = 0;
            virtual T get(get* e) = 0;
            virtual T set(set* e) = 0;
            virtual T grouping(grouping* e) = 0;
            virtual T literal(literal<T>* e) = 0;
            virtual T logical(logical* e) = 0;
            virtual T super(super* e) = 0;
            virtual T self(self* e) = 0;
            virtual T unary(unary* e) = 0;
            virtual T let(let* e) = 0;
    };
}

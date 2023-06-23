#pragma once

#include "expression.hpp"

namespace expression {
    class Assign;
    class Binary;
    class Call;
    class Get;
    class Set;
    class Grouping;
    template <class> class Literal;
    class Logical;
    class Super;
    class Self;
    class Unary;
    class Let;

    template <class T>
    class Visitor {
        public:
            virtual T assign(Assign* e) = 0;
            virtual T binary(Binary* e) = 0;
            virtual T call(Call* e) = 0;
            virtual T get(Get* e) = 0;
            virtual T set(Set* e) = 0;
            virtual T grouping(Grouping* e) = 0;
            virtual T literal(Literal<T>* e) = 0;
            virtual T logical(Logical* e) = 0;
            virtual T super(Super* e) = 0;
            virtual T self(Self* e) = 0;
            virtual T unary(Unary* e) = 0;
            virtual T let(Let* e) = 0;
    };
}

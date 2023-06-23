#pragma once

#include "expression.hpp"
#include "statement.hpp"

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
    class Variable;

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
            virtual T variable(Variable* e) = 0;
    };
};

namespace statement {
    class Block;
    class Class;
    class Expression;
    class Function;
    class If;
    class Return;
    class Let;
    class For;

    template <class T>
    class Visitor {
        public:
            virtual T block(Block* s) = 0;
            virtual T _class(Class* s) = 0;
            virtual T expression(Expression* s) = 0;
            virtual T function(Function* s) = 0;
            virtual T _if(If* s) = 0;
            virtual T _return(Return* s) = 0;
            virtual T let(Let* s) = 0;
            virtual T _for(For* s) = 0;
    };
};

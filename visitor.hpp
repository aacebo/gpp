#ifndef VISITOR_H
#define VISITOR_H

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
            virtual T visit_assign(Assign* e) = 0;
            virtual T visit_binary(Binary* e) = 0;
            virtual T visit_call(Call* e) = 0;
            virtual T visit_get(Get* e) = 0;
            virtual T visit_set(Set* e) = 0;
            virtual T visit_grouping(Grouping* e) = 0;
            virtual T visit_literal(Literal<T>* e) = 0;
            virtual T visit_logical(Logical* e) = 0;
            virtual T visit_super(Super* e) = 0;
            virtual T visit_self(Self* e) = 0;
            virtual T visit_unary(Unary* e) = 0;
            virtual T visit_variable(Variable* e) = 0;
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
    class Use;

    template <class T>
    class Visitor {
        public:
            virtual T visit_block(Block* s) = 0;
            virtual T visit_class(Class* s) = 0;
            virtual T visit_expression(Expression* s) = 0;
            virtual T visit_function(Function* s) = 0;
            virtual T visit_if(If* s) = 0;
            virtual T visit_return(Return* s) = 0;
            virtual T visit_let(Let* s) = 0;
            virtual T visit_for(For* s) = 0;
            virtual T visit_use(Use* s) = 0;
    };
};

#endif
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>

#include "visitor.hpp"
#include "scope.hpp"
#include "var.hpp"

using namespace std;

namespace interpreter {
    class Interpreter : public expression::Visitor<any>, public statement::Visitor<void> {
        scope::Scope* _scope = new scope::Scope();

        public:
            Interpreter() = default;
            ~Interpreter() { delete this->_scope; }

        private:
            any evaluate(expression::Expression);
            void execute(statement::Statement);

            void visit_let(statement::Let*);
            void visit_if(statement::If*);

            any visit_assign(expression::Assign*);
            any visit_literal(expression::Literal<any>*);
            any visit_self(expression::Self*);
            any visit_variable(expression::Variable*);
    };
};

#endif
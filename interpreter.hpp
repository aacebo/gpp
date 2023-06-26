#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>

#include "visitor.hpp"
#include "scope.hpp"
#include "return.hpp"
#include "var.hpp"
#include "callable.hpp"
#include "function.hpp"

using namespace std;

namespace interpreter {
    class Interpreter : public expression::Visitor<any>, public statement::Visitor<void> {
        scope::Scope* _scope = new scope::Scope();

        public:
            Interpreter() = default;
            ~Interpreter() { delete this->_scope; }

            any evaluate(expression::Expression);
            void execute(statement::Statement);
            void execute_block(vector<statement::Statement>, scope::Scope*);

        private:
            void visit_block(statement::Block*);
            void visit_let(statement::Let*);
            void visit_if(statement::If*);
            void visit_function(statement::Function*);
            void visit_return(statement::Return*);

            any visit_assign(expression::Assign*);
            any visit_literal(expression::Literal<any>*);
            any visit_self(expression::Self*);
            any visit_variable(expression::Variable*);
            any visit_call(expression::Call*);
    };
};

#endif
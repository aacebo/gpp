#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>
#include <vector>

#include "scope.hpp"
#include "return.hpp"
#include "var.hpp"
#include "expression.hpp"
#include "statement.hpp"

using namespace std;

namespace interpreter {
    class Interpreter {
        scope::Scope* scope = new scope::Scope();

        public:
            Interpreter() = default;
            ~Interpreter() { delete this->scope; }

            scope::Var* evaluate(expression::Expression*);
            void execute(statement::Statement*);
            void execute_block(vector<statement::Statement*>, scope::Scope*);

        private:
            void check_number_ops(token::Token*, scope::Var*);
            void check_number_ops(token::Token*, scope::Var*, scope::Var*);

            // Expressions
            scope::Var* visit_assign(expression::Assign*);
            scope::Var* visit_binary(expression::Binary*);
            scope::Var* visit_call(expression::Call*);
            scope::Var* visit_get(expression::Get*);
            scope::Var* visit_set(expression::Set*);
            scope::Var* visit_grouping(expression::Grouping*);
            scope::Var* visit_literal(expression::Literal<any>*);
            scope::Var* visit_logical(expression::Logical*);
            scope::Var* visit_super(expression::Super*);
            scope::Var* visit_self(expression::Self*);
            scope::Var* visit_unary(expression::Unary*);
            scope::Var* visit_variable(expression::Variable*);

            // Statements
            void visit_block(statement::Block*);
            void visit_function(statement::Function*);
            void visit_class(statement::Class*);
            void visit_expression(statement::Expression*);
            void visit_if(statement::If*);
            void visit_return(statement::Return*);
            void visit_let(statement::Let*);
            void visit_for(statement::For*);
            void visit_use(statement::Use*);
    };
};

#endif
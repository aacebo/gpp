#include "interpreter.hpp"

namespace interpreter {
    any Interpreter::evaluate(expression::Expression expr) {
        return expr.accept(this);
    }

    void Interpreter::execute(statement::Statement stmt) {
        return stmt.accept(this);
    }

    void Interpreter::visit_let(statement::Let* stmt) {
        any value;

        if (stmt->init) {
            value = this->evaluate(*stmt->init);
        }

        this->_scope->define(stmt->name, value);
    }

    void Interpreter::visit_if(statement::If* stmt) {
        if (scope::Var(this->evaluate(stmt->condition)).is_truthy()) {
            this->execute(*stmt->then_branch);
        } else if (stmt->else_branch) {
            this->execute(*stmt->else_branch);
        }
    }

    any Interpreter::visit_assign(expression::Assign* expr) {
        auto value = this->evaluate(expr->value);
        this->_scope->assign(expr->name, value);
        return value;
    }

    any Interpreter::visit_literal(expression::Literal<any>* expr) {
        return expr->value;
    }

    any Interpreter::visit_self(expression::Self* expr) {
        return this->_scope->get(expr->keyword)->value;
    }

    any Interpreter::visit_variable(expression::Variable* expr) {
        return this->_scope->get(expr->name)->value;
    }
};

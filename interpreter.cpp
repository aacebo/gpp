#include "interpreter.hpp"

namespace interpreter {
    any Interpreter::evaluate(expression::Expression expr) {
        return expr.accept(this);
    }

    void Interpreter::execute(statement::Statement stmt) {
        return stmt.accept(this);
    }

    void Interpreter::execute_block(vector<statement::Statement> stmts, scope::Scope* scope) {
        auto prev = this->_scope;

        try {
            this->_scope = scope;

            for (auto stmt : stmts) {
                this->execute(stmt);
            }
        } catch (error::RuntimeError err) {
            this->_scope = prev;
            throw err;
        }
    }

    void Interpreter::visit_block(statement::Block* stmt) {
        this->execute_block(stmt->stmts, new scope::Scope(this->_scope));
    }

    void Interpreter::visit_let(statement::Let* stmt) {
        any value;

        if (stmt->init) {
            value = this->evaluate(*stmt->init);
        }

        this->_scope->define(stmt->name.value, value);
    }

    void Interpreter::visit_if(statement::If* stmt) {
        if (scope::Var(this->evaluate(stmt->condition)).is_truthy()) {
            this->execute(*stmt->then_branch);
        } else if (stmt->else_branch) {
            this->execute(*stmt->else_branch);
        }
    }

    void Interpreter::visit_function(statement::Function* stmt) {
        auto fn = new scope::Function(*stmt, this->_scope);
        this->_scope->define(stmt->name.value, fn);
    }

    void Interpreter::visit_return(statement::Return* stmt) {
        any value;

        if (stmt->value) {
            value = this->evaluate(*stmt->value);
        }

        throw scope::Return(value);
    }

    any Interpreter::visit_assign(expression::Assign* expr) {
        auto value = this->evaluate(expr->value);
        this->_scope->assign(expr->name.value, value);
        return value;
    }

    any Interpreter::visit_literal(expression::Literal<any>* expr) {
        return expr->value;
    }

    any Interpreter::visit_self(expression::Self* expr) {
        return this->_scope->get(expr->keyword.value)->value;
    }

    any Interpreter::visit_variable(expression::Variable* expr) {
        return this->_scope->get(expr->name.value)->value;
    }

    any Interpreter::visit_call(expression::Call* expr) {
        any name = this->evaluate(expr->name);
        vector<any> args;

        for (auto arg : expr->args) {
            args.push_back(this->evaluate(arg));
        }

        if (!any_cast<scope::Callable*>(name)) {
            throw error::RuntimeError(
                expr->paren.ln,
                expr->paren.start,
                expr->paren.end,
                "can only call functions/classes"
            );
        }

        auto fn = any_cast<scope::Callable*>(name);
        return fn->call(this, args);
    }
};

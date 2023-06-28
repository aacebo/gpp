#include "interpreter.hpp"

namespace interpreter {
    void Interpreter::run(vector<statement::Statement*> stmts) {
        for (auto stmt : stmts) {
            this->execute(stmt);
        }
    }

    scope::Var* Interpreter::evaluate(expression::Expression* expr) {
        cout << expr->to_string() << endl;

        switch (expr->type) {
            case expression::Type::Assign:
                return this->visit_assign(dynamic_cast<expression::Assign*>(expr));
            case expression::Type::Binary:
                return this->visit_binary(dynamic_cast<expression::Binary*>(expr));
            case expression::Type::Call:
                return this->visit_call(dynamic_cast<expression::Call*>(expr));
            case expression::Type::Get:
                return this->visit_get(dynamic_cast<expression::Get*>(expr));
            case expression::Type::Set:
                return this->visit_set(dynamic_cast<expression::Set*>(expr));
            case expression::Type::Grouping:
                return this->visit_grouping(dynamic_cast<expression::Grouping*>(expr));
            case expression::Type::Literal:
                return this->visit_literal(dynamic_cast<expression::Literal*>(expr));
            case expression::Type::Logical:
                return this->visit_logical(dynamic_cast<expression::Logical*>(expr));
            case expression::Type::Super:
                return this->visit_super(dynamic_cast<expression::Super*>(expr));
            case expression::Type::Self:
                return this->visit_self(dynamic_cast<expression::Self*>(expr));
            case expression::Type::Unary:
                return this->visit_unary(dynamic_cast<expression::Unary*>(expr));
            case expression::Type::Variable:
                return this->visit_variable(dynamic_cast<expression::Variable*>(expr));
            default:
                throw runtime_error("expression type \"" + expr->to_string() + "\" is not supported");
        }
    }

    void Interpreter::execute(statement::Statement* stmt) {
        cout << stmt->to_string() << endl;

        switch (stmt->type) {
            case statement::Type::Block:
                return this->visit_block(dynamic_cast<statement::Block*>(stmt));
            case statement::Type::Function:
                return this->visit_function(dynamic_cast<statement::Function*>(stmt));
            case statement::Type::Class:
                return this->visit_class(dynamic_cast<statement::Class*>(stmt));
            case statement::Type::Expression:
                return this->visit_expression(dynamic_cast<statement::Expression*>(stmt));
            case statement::Type::If:
                return this->visit_if(dynamic_cast<statement::If*>(stmt));
            case statement::Type::Print:
                return this->visit_print(dynamic_cast<statement::Print*>(stmt));
            case statement::Type::Return:
                return this->visit_return(dynamic_cast<statement::Return*>(stmt));
            case statement::Type::Let:
                return this->visit_let(dynamic_cast<statement::Let*>(stmt));
            case statement::Type::For:
                return this->visit_for(dynamic_cast<statement::For*>(stmt));
            case statement::Type::Use:
                return this->visit_use(dynamic_cast<statement::Use*>(stmt));
            default:
                throw runtime_error("statement type \"" + stmt->to_string() + "\" is not supported");
        }
    }

    void Interpreter::execute_block(vector<statement::Statement*> stmts, scope::Scope* scope) {
        auto prev = this->scope;

        try {
            this->scope = scope;

            for (auto stmt : stmts) {
                this->execute(stmt);
            }

            this->scope = prev;
        } catch (exception e) {
            this->scope = prev;
            throw e;
        }
    }

    void Interpreter::check_number_ops(token::Token* op, scope::Var* value) {
        if (value->is_number()) return;

        throw error::RuntimeError(
            op->ln,
            op->start,
            op->end,
            "operand must be a number"
        );
    }

    void Interpreter::check_number_ops(token::Token* op, scope::Var* left, scope::Var* right) {
        if (left->is_number() && right->is_number()) return;

        throw error::RuntimeError(
            op->ln,
            op->start,
            op->end,
            "operands must be numbers"
        );
    }

    // Expressions

    scope::Var* Interpreter::visit_assign(expression::Assign* expr) {
        auto var = this->evaluate(expr->value);
        this->scope->assign(expr->name->value, var);
        return var;
    }

    scope::Var* Interpreter::visit_binary(expression::Binary* expr) {
        auto left = this->evaluate(expr->left);
        auto right = this->evaluate(expr->right);

        switch (expr->op->type) {
            case token::Type::NotEq:
                return new scope::Var(*left != *right);
            case token::Type::EqEq:
                return new scope::Var(*left == *right);
            case token::Type::Gt:
                this->check_number_ops(expr->op, left, right);
                return new scope::Var(*left > *right);
            case token::Type::GtEq:
                this->check_number_ops(expr->op, left, right);
                return new scope::Var(*left >= *right);
            case token::Type::Lt:
                this->check_number_ops(expr->op, left, right);
                return new scope::Var(*left < *right);
            case token::Type::LtEq:
                this->check_number_ops(expr->op, left, right);
                return new scope::Var(*left <= *right);
            case token::Type::Plus:
                this->check_number_ops(expr->op, left, right);
                return new scope::Var(*left + *right);
            case token::Type::PlusEq:
                this->check_number_ops(expr->op, left, right);
                *left += *right;
                return left;
            case token::Type::Minus:
                this->check_number_ops(expr->op, left, right);
                return new scope::Var(*left - *right);
            case token::Type::MinusEq:
                this->check_number_ops(expr->op, left, right);
                *left -= *right;
                return left;
            case token::Type::Slash:
                this->check_number_ops(expr->op, left, right);
                return new scope::Var(*left / *right);
            case token::Type::SlashEq:
                this->check_number_ops(expr->op, left, right);
                *left /= *right;
                return left;
            case token::Type::Star:
                this->check_number_ops(expr->op, left, right);
                return new scope::Var(*left * *right);
            case token::Type::StarEq:
                this->check_number_ops(expr->op, left, right);
                *left *= *right;
                return left;
            default:
                throw runtime_error("binary expression of type \"" + token::type_to_string(expr->op->type) + "\" is not supported");
        }
    }

    scope::Var* Interpreter::visit_call(expression::Call* expr) {
        vector<scope::Var*> args;
        auto callee = this->evaluate(expr->name);

        for (auto arg : expr->args) {
            args.push_back(this->evaluate(arg));
        }

        if (!callee->is_type<Function*>()) {
            throw new error::RuntimeError(
                expr->paren->ln,
                expr->paren->start,
                expr->paren->end,
                "only functions can be called"
            );
        }

        auto fn = callee->to_type<Function*>();

        if (fn->declaration->params.size() != args.size()) {
            throw new error::RuntimeError(
                expr->paren->ln,
                expr->paren->start,
                expr->paren->end,
                "expected " + to_string(fn->declaration->params.size()) +
                " arguments, received " + to_string(args.size())
            );
        }

        return fn->call(args);
    }

    scope::Var* Interpreter::visit_get(expression::Get* expr) {
        auto object = this->evaluate(expr->object);

        if (!object->is_type<Instance*>()) {
            throw new error::RuntimeError(
                expr->name->ln,
                expr->name->start,
                expr->name->end,
                "only instances have fields"
            );
        }

        auto instance = object->to_type<Instance*>();
        return instance->get(expr->name);
    }

    scope::Var* Interpreter::visit_set(expression::Set* expr) {
        auto object = this->evaluate(expr->object);

        if (!object->is_type<Instance*>()) {
            throw new error::RuntimeError(
                expr->name->ln,
                expr->name->start,
                expr->name->end,
                "only instances have fields"
            );
        }

        auto instance = object->to_type<Instance*>();
        auto value = this->evaluate(expr->value);
        instance->set(expr->name, value);

        return value;
    }

    scope::Var* Interpreter::visit_grouping(expression::Grouping* expr) {
        return this->evaluate(expr);
    }

    scope::Var* Interpreter::visit_literal(expression::Literal* expr) {
        return new scope::Var(expr->value);
    }

    scope::Var* Interpreter::visit_logical(expression::Logical* expr) {
        auto left = this->evaluate(expr->left);

        if (expr->op->type == token::Type::Or) {
            if (left->is_truthy()) {
                return left;
            }
        } else {
            if (!left->is_truthy()) {
                return left;
            }
        }

        return this->evaluate(expr->right);
    }

    scope::Var* Interpreter::visit_super(expression::Super* expr) {
        auto super = this->scope->get("super")->to_type<Class*>();
        auto instance = this->scope->get("self")->to_type<Instance*>();
        auto method = super->get_method(expr->method->value);

        if (!method) {
            throw new error::RuntimeError(
                expr->method->ln,
                expr->method->start,
                expr->method->end,
                "undefined property \"" + expr->method->value + "\""
            );
        }

        return new scope::Var(method->bind(instance));
    }

    scope::Var* Interpreter::visit_self(expression::Self* expr) {
        return this->scope->get(expr->keyword->value);
    }

    scope::Var* Interpreter::visit_unary(expression::Unary* expr) {
        auto right = this->evaluate(expr->right);

        switch (expr->op->type) {
            case token::Type::Not:
                return new scope::Var(!right->is_truthy());
            case token::Type::Minus:
                this->check_number_ops(expr->op, right);
                return new scope::Var(-*right);
            default:
                throw runtime_error("unary expression of type \"" + token::type_to_string(expr->op->type) + "\" is not supported");
        }
    }

    scope::Var* Interpreter::visit_variable(expression::Variable* expr) {
        return this->scope->get(expr->name->value);
    }

    // Statements

    void Interpreter::visit_block(statement::Block* stmt) {
        auto scope = new scope::Scope();
        this->execute_block(stmt->stmts, scope);
        delete scope;
    }

    void Interpreter::visit_function(statement::Function* stmt) {
        if (this->scope->has(stmt->name->value)) {
            if (this->scope->get(stmt->name->value)->is_type<Function*>()) {
                throw new error::RuntimeError(
                    stmt->name->ln,
                    stmt->name->start,
                    stmt->name->end,
                    "duplicate function declaration \"" + stmt->name->value + "\""
                );
            }
        }

        auto fn = new Function(stmt, this->scope);
        this->scope->define(stmt->name->value, new scope::Var(fn));
    }

    void Interpreter::visit_class(statement::Class* stmt) {
        scope::Var* super = NULL;

        if (stmt->super) {
            super = this->evaluate(stmt->super);

            if (!super->is_type<Class*>()) {
                throw new error::RuntimeError(
                    stmt->super->name->ln,
                    stmt->super->name->start,
                    stmt->super->name->end,
                    "inherited class must be of type class"
                );
            }
        }

        this->scope->define(stmt->name->value, NULL);

        if (stmt->super != NULL) {
            this->scope = new scope::Scope(this->scope);
            this->scope->define("super", super);
        }

        unordered_map<string, Function*> methods;

        for (auto method : stmt->methods) {
            methods[method->name->value] = new Function(
                method,
                this->scope
            );
        }

        auto _class = new Class(
            stmt->name->value,
            super ? super->to_type<Class*>() : NULL,
            methods
        );

        if (super) {
            this->scope = this->scope->parent;
        }

        this->scope->assign(stmt->name->value, new scope::Var(_class));
    }

    void Interpreter::visit_expression(statement::Expression* stmt) {
        this->evaluate(stmt->exp);
    }

    void Interpreter::visit_if(statement::If* stmt) {
        auto var = this->evaluate(stmt->condition);

        if (var->is_truthy()) {
            this->execute(stmt->then_branch);
        } else if (stmt->else_branch) {
            this->execute(stmt->else_branch);
        }
    }

    void Interpreter::visit_print(statement::Print* stmt) {
        auto var = this->evaluate(stmt->expr);
        auto value = var->to_string();

        if (var->is_type<Function*>()) {
            value = var->to_type<Function*>()->to_string();
        } else if (var->is_type<Class*>()) {
            value = var->to_type<Class*>()->to_string();
        } else if (var -> is_type<Instance*>()) {
            value = var->to_type<Instance*>()->to_string();
        }

        cout << value;
    }

    void Interpreter::visit_return(statement::Return* stmt) {
        auto var = (stmt->value) ? this->evaluate(stmt->value) : new scope::Var();
        throw new scope::Return(var);
    }

    void Interpreter::visit_let(statement::Let* stmt) {
        auto var = (stmt->init) ? this->evaluate(stmt->init) : new scope::Var();
        this->scope->define(stmt->name->value, var);
    }

    void Interpreter::visit_for(statement::For* stmt) {
        while (this->evaluate(stmt->condition)->is_truthy()) {
            this->execute(stmt->body);
        }
    }

    void Interpreter::visit_use(statement::Use* stmt) {
    }
};

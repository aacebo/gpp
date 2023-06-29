#include "interpreter.hpp"

namespace interpreter {
    Interpreter::Interpreter() {
        this->scope = new Scope();
    }

    Interpreter::~Interpreter() {
        delete this->scope;
    }

    void Interpreter::run(vector<statement::Statement*> stmts) {
        for (auto stmt : stmts) {
            this->execute(stmt);
        }
    }

    Var* Interpreter::evaluate(expression::Expression* expr) {
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

    void Interpreter::execute_block(vector<statement::Statement*> stmts, Scope* scope) {
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

    void Interpreter::check_number_ops(token::Token* op, Var* value) {
        if (value->is_number()) return;

        throw new error::RuntimeError(
            op->ln,
            op->start,
            op->end,
            "operand must be a number"
        );
    }

    void Interpreter::check_number_ops(token::Token* op, Var* left, Var* right) {
        if (left->is_number() && right->is_number()) return;

        throw new error::RuntimeError(
            op->ln,
            op->start,
            op->end,
            "operands must be numbers"
        );
    }

    // Expressions

    Var* Interpreter::visit_assign(expression::Assign* expr) {
        auto var = this->evaluate(expr->value);

        if (!this->scope->has(expr->name->value)) {
            throw new error::RuntimeError(
                expr->name->ln,
                expr->name->start,
                expr->name->end,
                "undefined variable \"" + expr->name->value + "\""
            );
        }

        auto existing = this->scope->get(expr->name->value);

        if (existing->is_const) {
            throw new error::RuntimeError(
                expr->name->ln,
                expr->name->start,
                expr->name->end,
                "cannot assign to a constant"
            );
        }

        if (var->type != Type::Nil && var->type != existing->type) {
            throw new error::RuntimeError(
                expr->name->ln,
                expr->name->start,
                expr->name->end,
                "expected type \"" + type_to_string(existing->type) +
                "\", received \"" + type_to_string(var->type) + "\""
            );
        }

        if (var->type == Type::Nil && !existing->is_optional) {
            throw new error::RuntimeError(
                expr->name->ln,
                expr->name->start,
                expr->name->end,
                "variable of type \"" + type_to_string(existing->type) +
                "\" is not optional"
            );
        }

        var->is_const = existing->is_const;
        var->is_optional = existing->is_optional;
        this->scope->assign(expr->name->value, var);

        return var;
    }

    Var* Interpreter::visit_binary(expression::Binary* expr) {
        auto left = this->evaluate(expr->left);
        auto right = this->evaluate(expr->right);

        switch (expr->op->type) {
            case token::Type::NotEq:
                return new Var(Type::Bool, *left != *right);
            case token::Type::EqEq:
                return new Var(Type::Bool, *left == *right);
            case token::Type::Gt:
                this->check_number_ops(expr->op, left, right);
                return new Var(Type::Bool, *left > *right);
            case token::Type::GtEq:
                this->check_number_ops(expr->op, left, right);
                return new Var(Type::Bool, *left >= *right);
            case token::Type::Lt:
                this->check_number_ops(expr->op, left, right);
                return new Var(Type::Bool, *left < *right);
            case token::Type::LtEq:
                this->check_number_ops(expr->op, left, right);
                return new Var(Type::Bool, *left <= *right);
            case token::Type::Plus:
                this->check_number_ops(expr->op, left, right);
                return new Var(Type::Number, *left + *right);
            case token::Type::PlusEq:
                this->check_number_ops(expr->op, left, right);
                *left += *right;
                return left;
            case token::Type::Minus:
                this->check_number_ops(expr->op, left, right);
                return new Var(Type::Number, *left - *right);
            case token::Type::MinusEq:
                this->check_number_ops(expr->op, left, right);
                *left -= *right;
                return left;
            case token::Type::Slash:
                this->check_number_ops(expr->op, left, right);
                return new Var(Type::Number, *left / *right);
            case token::Type::SlashEq:
                this->check_number_ops(expr->op, left, right);
                *left /= *right;
                return left;
            case token::Type::Star:
                this->check_number_ops(expr->op, left, right);
                return new Var(Type::Number, *left * *right);
            case token::Type::StarEq:
                this->check_number_ops(expr->op, left, right);
                *left *= *right;
                return left;
            default:
                throw runtime_error("binary expression of type \"" + token::type_to_string(expr->op->type) + "\" is not supported");
        }
    }

    Var* Interpreter::visit_call(expression::Call* expr) {
        vector<Var*> args;
        auto callee = this->evaluate(expr->name);

        for (auto arg : expr->args) {
            args.push_back(this->evaluate(arg));
        }

        if (!callee->is_function()) {
            throw new error::RuntimeError(
                expr->paren->ln,
                expr->paren->start,
                expr->paren->end,
                "only functions can be called"
            );
        }

        auto fn = callee->to_function();

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

    Var* Interpreter::visit_get(expression::Get* expr) {
        auto object = this->evaluate(expr->object);

        if (!object->is_instance()) {
            throw new error::RuntimeError(
                expr->name->ln,
                expr->name->start,
                expr->name->end,
                "only instances have fields"
            );
        }

        auto instance = object->to_instance();
        return instance->get(expr->name);
    }

    Var* Interpreter::visit_set(expression::Set* expr) {
        auto object = this->evaluate(expr->object);

        if (!object->is_instance()) {
            throw new error::RuntimeError(
                expr->name->ln,
                expr->name->start,
                expr->name->end,
                "only instances have fields"
            );
        }

        auto instance = object->to_instance();
        auto value = this->evaluate(expr->value);
        instance->set(expr->name, value);

        return value;
    }

    Var* Interpreter::visit_grouping(expression::Grouping* expr) {
        return this->evaluate(expr);
    }

    Var* Interpreter::visit_literal(expression::Literal* expr) {
        return new Var(type_info_to_type(expr->value.type()), expr->value);
    }

    Var* Interpreter::visit_logical(expression::Logical* expr) {
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

    Var* Interpreter::visit_super(expression::Super* expr) {
        auto super = this->scope->get("super")->to_class();
        auto instance = this->scope->get("self")->to_instance();
        auto method = super->get_method(expr->method->value);

        if (!method) {
            throw new error::RuntimeError(
                expr->method->ln,
                expr->method->start,
                expr->method->end,
                "undefined property \"" + expr->method->value + "\""
            );
        }

        return new Var(Type::Instance, method->bind(instance));
    }

    Var* Interpreter::visit_self(expression::Self* expr) {
        try {
            return this->scope->get(expr->keyword->value);
        } catch (runtime_error& e) {
            throw new error::RuntimeError(
                expr->keyword->ln,
                expr->keyword->start,
                expr->keyword->end,
                e.what()
            );
        }
    }

    Var* Interpreter::visit_unary(expression::Unary* expr) {
        auto right = this->evaluate(expr->right);

        switch (expr->op->type) {
            case token::Type::Not:
                return new Var(Type::Bool, !right->is_truthy());
            case token::Type::Minus:
                this->check_number_ops(expr->op, right);
                return new Var(Type::Number, -*right);
            default:
                throw runtime_error("unary expression of type \"" + token::type_to_string(expr->op->type) + "\" is not supported");
        }
    }

    Var* Interpreter::visit_variable(expression::Variable* expr) {
        try {
            return this->scope->get(expr->name->value);
        } catch (runtime_error& e) {
            throw new error::RuntimeError(
                expr->name->ln,
                expr->name->start,
                expr->name->end,
                e.what()
            );
        }
    }

    // Statements

    void Interpreter::visit_block(statement::Block* stmt) {
        auto scope = new Scope();
        this->execute_block(stmt->stmts, scope);
        delete scope;
    }

    void Interpreter::visit_function(statement::Function* stmt) {
        if (this->scope->has(stmt->name->value)) {
            if (this->scope->get(stmt->name->value)->is_function()) {
                throw new error::RuntimeError(
                    stmt->name->ln,
                    stmt->name->start,
                    stmt->name->end,
                    "duplicate function declaration \"" + stmt->name->value + "\""
                );
            }
        }

        auto fn = new Function(stmt, this->scope);
        this->scope->define(stmt->name->value, new Var(Type::Function, fn, true, false));
    }

    void Interpreter::visit_class(statement::Class* stmt) {
        Var* super = NULL;

        if (stmt->super) {
            super = this->evaluate(stmt->super);

            if (!super->is_class()) {
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
            this->scope = new Scope(this->scope);
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
            super ? super->to_class() : NULL,
            methods
        );

        if (super) {
            this->scope = this->scope->parent;
        }

        this->scope->assign(stmt->name->value, new Var(Type::Class, _class, true, false));
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
        cout << var->to_string();
    }

    void Interpreter::visit_return(statement::Return* stmt) {
        auto var = (stmt->value) ? this->evaluate(stmt->value) : NULL;
        throw new Return(var);
    }

    void Interpreter::visit_let(statement::Let* stmt) {
        if (this->scope->has(stmt->name->value)) {
            throw new error::RuntimeError(
                stmt->name->ln,
                stmt->name->start,
                stmt->name->end,
                "duplicate variable declaration \"" + stmt->name->value + "\""
            );
        }

        if (!stmt->init && !stmt->_type) {
            throw new error::RuntimeError(
                stmt->name->ln,
                stmt->name->start,
                stmt->name->end,
                "variable must have a type or initialized value"
            );
        }

        if (stmt->_type && !stmt->init && !stmt->optional) {
            throw new error::RuntimeError(
                stmt->name->ln,
                stmt->name->start,
                stmt->name->end,
                "default value for uninitialized variable is 'nil', try using '" 
                + token::type_to_string(stmt->_type->type) + "?'"
            );
        }

        auto var = (stmt->init) ? this->evaluate(stmt->init) : new Var(token_type_to_type(stmt->_type->type));

        if (stmt->_type) {
            if (var->type != Type::Nil && token_type_to_type(stmt->_type->type) != var->type) {
                throw new error::RuntimeError(
                    stmt->_type->ln,
                    stmt->_type->start,
                    stmt->_type->end,
                    "expected type \"" + type_to_string(token_type_to_type(stmt->_type->type)) +
                    "\", received \"" + type_to_string(var->type) + "\""
                );
            }

            if (var->type == Type::Nil && !stmt->optional) {
                throw new error::RuntimeError(
                    stmt->_type->ln,
                    stmt->_type->start,
                    stmt->_type->end,
                    "variable of type \"" + type_to_string(token_type_to_type(stmt->_type->type)) +
                    "\" is not optional"
                );
            }

            var->type = token_type_to_type(stmt->_type->type);
        }

        if (stmt->keyword->type == token::Type::Const) {
            var->is_const = true;
        }

        if (stmt->optional) {
            var->is_optional = true;
        }

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

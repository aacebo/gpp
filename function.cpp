#include "interpreter.hpp"

namespace interpreter {
    Function::Function(
        statement::Function* declaration,
        Scope* parent
    ) : declaration(declaration), parent(parent) {

    }

    string Function::to_string() {
        return "<fn " + this->declaration->name->value + ">";
    }

    Function* Function::bind(Instance* instance) {
        auto scope = new Scope(this->parent);
        scope->define("self", new Var(Type::Instance, instance, true, false));
        return new Function(this->declaration, scope);
    }

    Var* Function::call(vector<Var*> args) {
        Var* return_value = NULL;
        auto scope = new Scope(this->parent);

        for (int i = 0; i < this->declaration->params.size(); i++) {
            scope->define(
                this->declaration->params[i]->value,
                args[i]
            );
        }

        try {
            this->execute_block(this->declaration->body, scope);
        } catch (Return* r) {
            if (r->value) {
                return_value = new Var(r->value->type, r->value->value);
            }
        }

        if (!return_value && this->declaration->return_type && !this->declaration->optional) {
            throw new error::RuntimeError(
                this->declaration->return_type->ln,
                this->declaration->return_type->start,
                this->declaration->return_type->end,
                "expected return type \"" + this->declaration->return_type->value + "\", received 'nil'"
            );
        }

        if (return_value && !this->declaration->return_type) {
            throw new error::RuntimeError(
                this->declaration->name->ln,
                this->declaration->name->start,
                this->declaration->name->end,
                "function return type not specified"
            );
        }

        if (return_value && this->declaration->return_type) {
            if (return_value->type != Type::Nil && return_value->type != token_type_to_type(this->declaration->return_type->type)) {
                throw new error::RuntimeError(
                    this->declaration->return_type->ln,
                    this->declaration->return_type->start,
                    this->declaration->return_type->end,
                    "expected return type \"" + this->declaration->return_type->value + 
                    "\", received \"" + type_to_string(return_value->type) + "\""
                );
            }

            if (return_value->type == Type::Nil && !this->declaration->optional) {
                throw new error::RuntimeError(
                    this->declaration->return_type->ln,
                    this->declaration->return_type->start,
                    this->declaration->return_type->end,
                    "return type \"" + this->declaration->return_type->value + "\" is not optional"
                );
            }
        }

        return return_value ? return_value : new Var(Type::Nil);
    }
};

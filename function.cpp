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
        auto scope = new Scope(this->parent);

        for (int i = 0; i < this->declaration->params.size(); i++) {
            scope->define(
                this->declaration->params[i]->value,
                args[i]
            );
        }

        try {
            this->execute_block(this->declaration->body, scope);
            delete scope;
        } catch (Return* r) {
            delete scope;
            return r->value;
        }

        return NULL;
    }
};

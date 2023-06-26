#ifndef FUNCTION_H
#define FUNCTION_H

#include "class.hpp"
#include "statement.hpp"
#include "error.hpp"
#include "callable.hpp"
#include "scope.hpp"
#include "interpreter.hpp"

namespace scope {
    class Function : public Callable {
        const statement::Function _declaration;
        const bool _is_init = false;
        Scope* _scope;

        public:
            Function(
                statement::Function declaration,
                Scope* scope
            ) : _declaration(declaration), _scope(scope) { }

            Function(
                statement::Function declaration,
                Scope* scope,
                bool is_init
            ) : _declaration(declaration), _scope(scope), _is_init(is_init) { }

            string to_string() { return "<fn " + this->_declaration.name.value + ">"; }

            Function* bind(Instance* instance) {
                auto scope = new Scope(this->_scope);
                scope->define("self", instance);

                return new Function(
                    this->_declaration,
                    scope
                );
            }

            any call(interpreter::Interpreter* inter, vector<any> args) {
                auto scope = new Scope(this->_scope);

                for (int i = 0; i < this->_declaration.params.size(); i++) {
                    scope->define(
                        this->_declaration.params[i].value,
                        args[i]
                    );
                }

                try {
                    inter->execute_block(this->_declaration.body, scope);
                } catch (Return r) {
                    if (!this->_is_init) return scope->get("self")->get();
                    return r.value;
                }

                if (this->_is_init) return scope->get("self")->get();
                return NULL;
            }
    };
};

#endif
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>
#include <vector>
#include <unordered_map>

#include "scope.hpp"
#include "return.hpp"
#include "var.hpp"
#include "expression.hpp"
#include "statement.hpp"

using namespace std;

namespace interpreter {
    class Instance;

    class Interpreter {
        scope::Scope* scope = new scope::Scope();

        public:
            Interpreter() = default;
            ~Interpreter() { delete this->scope; }
            void run(vector<statement::Statement*>);

        protected:
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

    class Function : public Interpreter {
        public:
            statement::Function* declaration;
            scope::Scope* parent;

            Function(
                statement::Function* declaration,
                scope::Scope* parent
            ) : declaration(declaration), parent(parent) { }

            string to_string() { return "<fn " + this->declaration->name->value + ">"; }

            Function* bind(Instance* instance) {
                auto scope = new scope::Scope(this->parent);
                scope->define("self", new scope::Var(instance));
                return new Function(this->declaration, scope);
            }

            scope::Var* call(vector<scope::Var*> args) {
                auto scope = new scope::Scope(this->parent);

                for (int i = 0; i < this->declaration->params.size(); i++) {
                    scope->define(
                        this->declaration->params[i]->value,
                        args[i]
                    );
                }

                try {
                    this->execute_block(this->declaration->body, scope);
                    delete scope;
                } catch (scope::Return* r) {
                    delete scope;
                    return r->value;
                }

                return new scope::Var();
            }
    };

    class Class : public Interpreter {
        public:
            const string name;
            const unordered_map<string, Function*> methods;
            Class* super;

            Class(
                string name,
                Class* super,
                unordered_map<string, Function*> methods
            ) : name(name), super(super), methods(methods) { }

            string to_string() { return "[class " + this->name + "]"; }

            Function* get_method(string name) {
                if (this->methods.count(name) > 0) {
                    return this->methods.at(name);
                }

                if (this->super) {
                    return this->super->get_method(name);
                }

                return NULL;
            }
    };

    class Instance {
        Class* _class;
        unordered_map<string, scope::Var*> _fields;

        public:
            Instance(Class* _class) : _class(_class) { }

            string to_string() {
                string res = this->_class->name + " {\n";
                auto it = this->_fields.begin();

                while (it != this->_fields.end()) {
                    res += "\t" + it->first + ": " + it->second->to_string() + "";
                    it++;

                    if (it != this->_fields.end()) {
                        res += ",\n";
                    }
                }

                return res + "}";
            }

            void set(token::Token* name, scope::Var* value) {
                this->_fields[name->value] = value;
            }

            scope::Var* get(token::Token* name) {
                if (this->_fields.count(name->value) > 0) {
                    return this->_fields.at(name->value);
                }

                auto method = this->_class->get_method(name->value);

                if (method) {
                    return new scope::Var(method->bind(this));
                }

                throw new error::RuntimeError(
                    name->ln,
                    name->start,
                    name->end,
                    "undefined property \"" + name->value + "\""
                );
            }
    };
};

#endif
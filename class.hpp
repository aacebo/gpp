#ifndef CLASS_H
#define CLASS_H

#include <any>
#include <string>
#include <unordered_map>

#include "token.hpp"
#include "error.hpp"
#include "callable.hpp"
#include "scope.hpp"

using namespace std;

namespace scope {
    class Class : public Callable {
        public:
            const string name;
            const Class* parent;

            Class(string name, Class* parent) : name(name), parent(parent) { }

            string to_string() { return this->name; }
            any call(interpreter::Interpreter* interp, vector<any> args) {

            }
    };

    class Instance {
        unordered_map<string, Var<any>*> _fields;

        public:
            const Class* type;

            Instance(Class* type) : type(type) { }

            string to_string() { return this->type->name + " instance"; }

            Var<any>* get(token::Token name) {
                if (this->_fields.count(name.value) == 0) {
                    throw error::RuntimeError(
                        name.ln,
                        name.start,
                        name.end,
                        "undefined property \"" + name.value + "\" in class instance \"" + this->type->name + "\""
                    );
                }

                return this->_fields[name.value];
            }
    };
};

#endif
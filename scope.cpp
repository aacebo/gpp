#include "scope.hpp"

namespace value {
    Scope::Scope() {

    }

    Scope::Scope(Scope* parent) : parent(parent) {

    }

    bool Scope::has_local(const string& name) {
        return this->definitions.count(name) > 0;
    }

    bool Scope::has(const string& name) {
        if (this->has_local(name)) {
            return true;
        }

        if (this->parent) {
            return this->parent->has(name);
        }

        return false;
    }

    void Scope::define(const string& name, Definition def, Value value) {
        if (this->has_local(name)) {
            throw runtime_error("variable '" + name + "' is already defined in this scope");
        }

        this->definitions.insert({name, def});
        this->values.insert({name, value});
    }

    Definition& Scope::get_definition(const string& name) {
        if (this->has_local(name)) {
            return this->definitions.at(name);
        }

        if (this->parent) {
            return this->parent->get_definition(name);
        }

        throw runtime_error("variable '" + name + "' is not defined");
    }

    void Scope::assign(const string& name, Value value) {
        if (this->has_local(name)) {
            if (this->definitions.at(name).is_const) {
                throw runtime_error("cannot assign to constant");
            }

            if (
                !(this->definitions.at(name).is_optional && value.is_nil()) &&
                this->definitions.at(name).type != value.type
            ) {
                throw runtime_error(
                    "cannot assign value type '" + type_to_string(value.type) +
                    "' to variable type '" + type_to_string(this->definitions.at(name).type) + "'"
                );
            }

            this->values.insert({name, value});
            return;
        }

        if (this->parent) {
            return this->parent->assign(name, value);
        }

        throw runtime_error("cannot assign to undefined variable '" + name + "'");
    }

    Value& Scope::resolve(const string& name) {
        if (this->has_local(name)) {
            return this->values.at(name);
        }

        if (this->parent) {
            return this->parent->resolve(name);
        }

        throw runtime_error("cannot resolve undefined variable '" + name + "'");
    }
};

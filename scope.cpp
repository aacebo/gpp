#include "scope.hpp"

namespace scope {
    Scope::~Scope() {
        for (auto v : this->_values) {
            delete v.second;
        }
    }

    Var* Scope::get(string name) {
        if (this->has(name)) {
            return this->_values[name];
        }

        if (this->parent) {
            return this->parent->get(name);
        }

        throw runtime_error("undefined variable \"" + name + "\"");
    }

    bool Scope::has(string name) {
        return this->_values.count(name) == 1;
    }

    void Scope::define(string name, Var* var) {
        this->_values[name] = var;
    }

    void Scope::assign(string name, Var* var) {
        if (this->has(name)) {
            this->_values[name] = var;
            return;
        }

        if (this->parent) {
            return this->parent->assign(name, var);
        }

        throw runtime_error("undefined variable \"" + name + "\"");
    }
};

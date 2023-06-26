#include "scope.hpp"

namespace scope {
    Scope::~Scope() {
        for (auto v : this->_values) {
            delete v.second;
        }
    }

    Var<any>* Scope::get(string name) {
        if (this->has(name)) {
            return this->_values[name];
        }

        if (this->_outer) {
            return this->_outer->get(name);
        }

        throw runtime_error("undefined variable \"" + name + "\"");
    }

    bool Scope::has(string name) {
        return this->_values.count(name) == 1;
    }

    void Scope::define(string name, any value) {
        this->_values[name] = new Var(value);
    }

    void Scope::assign(string name, any value) {
        if (this->has(name)) {
            this->_values[name]->set(value);
            return;
        }

        if (this->_outer) {
            return this->_outer->assign(name, value);
        }

        throw runtime_error("undefined variable \"" + name + "\"");
    }
};

#include "scope.hpp"

namespace scope {
    any Scope::get(token::Token name) {
        if (this->has(name)) {
            return this->_values[name.value];
        }

        if (this->_outer) {
            return this->_outer->get(name);
        }

        throw new error::RuntimeError(
            name.ln,
            name.start,
            name.end,
            "undefined variable \"" + name.value + "\""
        );
    }

    bool Scope::has(token::Token name) {
        return this->_values.count(name.value) == 1;
    }

    void Scope::define(token::Token name, any value) {
        this->_values[name.value] = value;
    }

    void Scope::assign(token::Token name, any value) {
        if (this->has(name)) {
            this->_values[name.value] = value;
            return;
        }

        if (this->_outer) {
            return this->_outer->assign(name, value);
        }

        throw new error::RuntimeError(
            name.ln,
            name.start,
            name.end,
            "undefined variable \"" + name.value + "\""
        );
    }
};

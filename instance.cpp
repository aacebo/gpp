#include "interpreter.hpp"

namespace interpreter {
    Instance::Instance(Class* _class) : _class(_class) {

    }
    
    Instance::~Instance() {
        for (auto var : this->_fields) {
            delete var.second;
        }

        this->_fields.clear();
    }

    string Instance::to_string() {
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

    void Instance::set(token::Token* name, Var* value) {
        this->_fields[name->value] = value;
    }

    Var* Instance::get(token::Token* name) {
        if (this->_fields.count(name->value) > 0) {
            return this->_fields.at(name->value);
        }

        auto method = this->_class->get_method(name->value);

        if (method) {
            return new Var(Type::Function, method->bind(this), true, false);
        }

        throw new error::RuntimeError(
            name->ln,
            name->start,
            name->end,
            "undefined property \"" + name->value + "\""
        );
    }
};

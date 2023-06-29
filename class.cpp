#include "interpreter.hpp"

namespace interpreter {
    Class::Class(
        string name,
        Class* super,
        unordered_map<string, Function*> methods
    ) : name(name), super(super), methods(methods) {

    }

    Class::~Class() {
        for (auto fn : this->methods) {
            delete fn.second;
        }

        this->methods.clear();
    }

    string Class::to_string() {
        return "[class " + this->name + "]";
    }

    Function* Class::get_method(string name) {
        if (this->methods.count(name) > 0) {
            return this->methods.at(name);
        }

        if (this->super) {
            return this->super->get_method(name);
        }

        return NULL;
    }
};

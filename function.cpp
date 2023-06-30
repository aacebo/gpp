#include "function.hpp"

namespace value {
    Function::Function(FunctionType type, string name, int argc) : Object(ObjectType::Function), fn_type(type), name(name), argc(argc) { }
    Function::Function(string name, int argc) : Function(FunctionType::Default, name, argc) { }
    Closure::Closure(string name, int argc) : Function(FunctionType::Closure, name, argc) { }
    Method::Method(string name, int argc) : Function(FunctionType::Method, name, argc) { }
    Native::Native(string name, int argc) : Function(FunctionType::Native, name, argc) { }

    bool Function::is_closure() {
        return this->fn_type == FunctionType::Closure;
    }

    bool Function::is_method() {
        return this->fn_type == FunctionType::Method;
    }

    bool Function::is_native() {
        return this->fn_type == FunctionType::Native;
    }
};

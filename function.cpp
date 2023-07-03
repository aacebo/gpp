#include "function.hpp"

namespace value {
    Function::Function(
        FunctionType type
    ) : Object(ObjectType::Function),
        fn_type(type),
        name(""),
        return_type(value::Type::Nil) {

    }

    Function::Function(
        FunctionType type,
        string name,
        vector<value::Type> arg_types,
        value::Type return_type
    ) : Object(ObjectType::Function),
        fn_type(type),
        name(name),
        arg_types(arg_types),
        return_type(return_type) {

    }

    Function::Function(
        string name,
        vector<value::Type> arg_types,
        value::Type return_type
    ) : Function(
        FunctionType::Default,
        name,
        arg_types,
        return_type
    ) { }

    bool Function::is_default() {
        return this->fn_type == FunctionType::Default;
    }

    bool Function::is_closure() {
        return this->fn_type == FunctionType::Closure;
    }

    bool Function::is_method() {
        return this->fn_type == FunctionType::Method;
    }

    string Function::to_string() {
        return "<fn " + this->name + ">";
    }

    Closure::Closure() : Function(FunctionType::Closure) {
        
    }

    Closure::Closure(
        vector<value::Type> arg_types,
        value::Type return_type
    ) : Function(
        FunctionType::Closure,
        "",
        arg_types,
        return_type
    ) { }

    string Closure::to_string() {
        return "<fn (anonymous)>";
    }

    Method::Method(
        string name,
        vector<value::Type> arg_types,
        value::Type return_type
    ) : Function(
        FunctionType::Method,
        name,
        arg_types,
        return_type
    ) { }

    string Method::to_string() {
        return "<fn " + this->parent->name + "." + this->name + ">";
    }
};

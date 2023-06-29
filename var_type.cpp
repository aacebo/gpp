#include "interpreter.hpp"

namespace interpreter {
    Type type_info_to_type(const type_info& type) {
        if (type == typeid(string)) {
            return Type::String;
        } else if (type == typeid(float)) {
            return Type::Number;
        } else if (type == typeid(bool)) {
            return Type::Bool;
        } else if (type == typeid(Class*)) {
            return Type::Class;
        } else if (type == typeid(Instance*)) {
            return Type::Instance;
        } else if (type == typeid(Function*)) {
            return Type::Function;
        } else if (type == typeid(NULL)) {
            return Type::Nil;
        }

        throw runtime_error("invalid type info");
    }

    string type_to_string(Type type) {
        switch (type) {
            case Type::String:
                return "string";
            case Type::Number:
                return "number";
            case Type::Bool:
                return "bool";
            case Type::Class:
                return "class";
            case Type::Instance:
                return "instance";
            case Type::Function:
                return "function";
            case Type::Nil:
                return "nil";
        }
    }

    Type token_type_to_type(token::Type type) {
        switch (type) {
            case token::Type::String:
                return Type::String;
            case token::Type::Number:
                return Type::Number;
            case token::Type::Bool:
                return Type::Bool;
            case token::Type::Class:
                return Type::Class;
            case token::Type::Fn:
                return Type::Function;
            case token::Type::Nil:
                return Type::Nil;
            default:
                throw runtime_error("invalid type \"" + token::type_to_string(type) + "\"");
        }
    }
};

#include "value.hpp"

namespace value {
    Value::Value() : type(Type::Nil) {
        this->_value = nullptr;
    }

    Value::Value(bool value) : type(Type::Bool) {
        this->_value = value;
    }

    Value::Value(float value) : type(Type::Number) {
        this->_value = value;
    }

    Value::Value(Object* value) : type(Type::Object) {
        this->_value = value;
    }

    bool Value::is_bool() {
        return this->type == Type::Bool;
    }

    bool Value::to_bool() {
        return get<bool>(this->_value);
    }

    bool Value::is_number() {
        return this->type == Type::Number;
    }

    float Value::to_number() {
        return get<float>(this->_value);
    }

    bool Value::is_nil() {
        return this->type == Type::Nil;
    }

    nullptr_t Value::to_nil() {
        return get<nullptr_t>(this->_value);
    }

    bool Value::is_object() {
        return this->type == Type::Object;
    }

    Object* Value::to_object() {
        return get<Object*>(this->_value);
    }
};
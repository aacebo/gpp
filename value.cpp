#include "value.hpp"

namespace value {
    Value::Value() : type(Type::Nil) {
        this->value = nullptr;
    }

    Value::Value(string value) : type(Type::String) {
        this->value = value;
    }

    Value::Value(bool value) : type(Type::Bool) {
        this->value = value;
    }

    Value::Value(float value) : type(Type::Number) {
        this->value = value;
    }

    Value::Value(Object* value) : type(Type::Object) {
        this->value = value;
    }

    Value::~Value() {
        if (this->is_object()) {
            delete this->to_object();
        }
    }

    bool Value::is_string() {
        return this->type == Type::String;
    }

    string Value::to_string() {
        return get<string>(this->value);
    }

    bool Value::is_bool() {
        return this->type == Type::Bool;
    }

    bool Value::to_bool() {
        return get<bool>(this->value);
    }

    bool Value::is_number() {
        return this->type == Type::Number;
    }

    float Value::to_number() {
        return get<float>(this->value);
    }

    bool Value::is_nil() {
        return this->type == Type::Nil;
    }

    nullptr_t Value::to_nil() {
        return get<nullptr_t>(this->value);
    }

    bool Value::is_object() {
        return this->type == Type::Object;
    }

    Object* Value::to_object() {
        return get<Object*>(this->value);
    }
};

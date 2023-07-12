#include "value.hpp"

namespace value {
    Value::Value() : type(Type::Nil) {
        this->value = nullptr;
    }

    Value::Value(bool value) : type(Type::Bool) {
        this->value = value;
    }

    Value::Value(float value) : type(Type::Number) {
        this->value = value;
    }

    Value::Value(int value) : type(Type::Number) {
        this->value = static_cast<float>(value);
    }

    Value::Value(Object* value) : type(Type::Object) {
        this->value = value;
    }

    Value::Value(const Value& other) : type(other.type) {
        this->value = other.value;
    }

    Value::~Value() {
        // if (this->is_object()) {
        //     delete this->to_object();
        // }
    }

    Value Value::parse(any value) {
        if (value.type() == typeid(nullptr)) {
            return Value();
        } else if (value.type() == typeid(bool)) {
            return Value(any_cast<bool>(value));
        } else if (value.type() == typeid(float)) {
            return Value(any_cast<float>(value));
        } else if (value.type() == typeid(int)) {
            return Value(any_cast<int>(value));
        } else if (value.type() == typeid(Object*)) {
            return Value(any_cast<Object*>(value));
        }

        throw runtime_error("unsupported value type");
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
        return this->type == Type::Nil || holds_alternative<nullptr_t>(this->value);
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

    bool Value::is_string() {
        return this->to_object()->is_string();
    }

    String* Value::to_string() {
        return dynamic_cast<String*>(this->to_object());
    }

    string Value::as_string() {
        if (this->is_nil()) return "<nil>";
        if (this->is_bool()) return this->to_bool() == true ? "true" : "false";
        if (this->is_number()) return std::to_string(this->to_number());
        if (this->is_object() && this->is_string()) return this->to_string()->to_string();
        return "";
    }

    bool Value::is_truthy() {
        if (this->is_nil()) return false;
        if (this->is_bool()) return this->to_bool();
        return true;
    }

    Value& Value::operator=(const Value& other) {
        this->type = other.type;
        this->value = other.value;
        return *this;
    }

    bool Value::operator==(Value& other) {
        if (this->type != other.type) return false;
        if (this->is_nil() && other.is_nil()) return true;

        if (this->is_object() && this->is_string() && other.is_string()) {
            return this->to_string()->equals(*other.to_string());
        }

        if (this->is_number()) {
            return this->to_number() == other.to_number();
        } else if (this->is_bool()) {
            return this->to_bool() == other.to_bool();
        }

        return false;
    }

    bool Value::operator!=(Value& other) {
        return !this->operator==(other);
    }

    bool Value::operator>(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() > other.to_number();
    }

    bool Value::operator>=(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() >= other.to_number();
    }

    bool Value::operator<(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() < other.to_number();
    }

    bool Value::operator<=(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() <= other.to_number();
    }

    float Value::operator+(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() + other.to_number();
    }

    float Value::operator+=(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        auto value = this->to_number() + other.to_number();
        this->value = value;
        return value;
    }

    float Value::operator-() {
        if (!this->is_number()) {
            throw runtime_error("operand must be a number");
        }

        return -this->to_number();
    }

    float Value::operator-(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() - other.to_number();
    }

    float Value::operator-=(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        auto value = this->to_number() - other.to_number();
        this->value = value;
        return value;
    }

    float Value::operator*(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() * other.to_number();
    }

    float Value::operator*=(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        auto value = this->to_number() * other.to_number();
        this->value = value;
        return value;
    }

    float Value::operator/(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() / other.to_number();
    }

    float Value::operator/=(Value& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        auto value = this->to_number() / other.to_number();
        this->value = value;
        return value;
    }
};

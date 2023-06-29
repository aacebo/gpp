#include "interpreter.hpp"

namespace interpreter {
    Var::Var(Type type) : type(type), value(any(NULL)) { }
    Var::Var(Type type, any value) : type(type), value(value) { }
    Var::Var(Type type, any value, bool is_const, bool is_optional) : type(type), value(value), is_const(is_const), is_optional(is_optional) { }

    void Var::nil() { this->value = any(NULL); }
    bool Var::is_nil() { return this->value.type() == any(NULL).type(); }

    bool Var::is_string() { return this->type == Type::String; }
    string Var::to_string() {
        if (this->is_nil()) return "<nil>";
        if (this->is_string()) return any_cast<string>(this->value);
        if (this->is_number()) return std::to_string(this->to_number());
        if (this->is_bool()) return this->to_bool() ? "true" : "false";
        if (this->is_class()) return this->to_class()->to_string();
        if (this->is_instance()) return this->to_instance()->to_string();
        if (this->is_function()) return this->to_function()->to_string();
        return "";
    }

    bool Var::is_number() { return this->type == Type::Number; }
    float Var::to_number() { return any_cast<float>(this->value); }

    bool Var::is_bool() { return this->type == Type::Bool; }
    bool Var::to_bool() { return any_cast<bool>(this->value); }

    bool Var::is_class() { return this->type == Type::Class; }
    Class* Var::to_class() { return any_cast<Class*>(this->value); }

    bool Var::is_instance() { return this->type == Type::Instance; }
    Instance* Var::to_instance() { return any_cast<Instance*>(this->value); }

    bool Var::is_function() { return this->type == Type::Function; }
    Function* Var::to_function() { return any_cast<Function*>(this->value); }

    bool Var::is_truthy() {
        if (this->is_nil()) return false;
        if (this->is_bool()) {
            return this->to_bool();
        }

        return true;
    }

    bool Var::operator==(Var& other) {
        if (this->type != other.type) return false;
        if (this->is_nil() && other.is_nil()) return true;

        if (this->is_string()) {
            return this->to_string() == other.to_string();
        } else if (this->is_number()) {
            return this->to_number() == other.to_number();
        } else if (this->is_bool()) {
            return this->to_bool() == other.to_bool();
        }

        return false;
    }

    bool Var::operator!=(Var& other) {
        return !this->operator==(other);
    }

    bool Var::operator>(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() > other.to_number();
    }

    bool Var::operator>=(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() >= other.to_number();
    }

    bool Var::operator<(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() < other.to_number();
    }

    bool Var::operator<=(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() <= other.to_number();
    }

    float Var::operator+(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() + other.to_number();
    }

    float Var::operator+=(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        auto value = this->to_number() + other.to_number();
        this->value = value;
        return value;
    }

    float Var::operator-() {
        if (!this->is_number()) {
            throw runtime_error("operand must be a number");
        }

        return -this->to_number();
    }

    float Var::operator-(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() - other.to_number();
    }

    float Var::operator-=(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        auto value = this->to_number() - other.to_number();
        this->value = value;
        return value;
    }

    float Var::operator*(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() * other.to_number();
    }

    float Var::operator*=(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        auto value = this->to_number() * other.to_number();
        this->value = value;
        return value;
    }

    float Var::operator/(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        return this->to_number() / other.to_number();
    }

    float Var::operator/=(Var& other) {
        if (!this->is_number() || !other.is_number()) {
            throw runtime_error("both operands must be numbers");
        }

        auto value = this->to_number() / other.to_number();
        this->value = value;
        return value;
    }
};

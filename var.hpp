#ifndef VAR_H
#define VAR_H

#include <any>
#include <string>

#include "error.hpp"

using namespace std;

namespace scope {   
    class Var {
        public:
            any value;

            Var() { this->value = any(); }
            Var(any value) { this->value = value; }

            void nil() { this->value.reset(); }
            bool is_nil() { return !this->value.has_value(); }
            const type_info& type() { return this->value.type(); }

            bool is_string() { return this->type() == typeid(string); }
            string to_string() { return any_cast<string>(this->value); }

            bool is_number() { return this->type() == typeid(float); }
            float to_number() { return any_cast<float>(this->value); }

            bool is_bool() { return this->type() == typeid(bool); }
            bool to_bool() { return any_cast<bool>(this->value); }

            bool is_truthy() {
                if (this->is_nil()) return false;
                if (this->is_bool()) {
                    return this->to_bool();
                }

                return true;
            }

            bool operator==(Var& other) {
                if (this->type() != other.type()) return false;
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

            bool operator!=(Var& other) {
                return !this->operator==(other);
            }

            bool operator>(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                return this->to_number() > other.to_number();
            }

            bool operator>=(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                return this->to_number() >= other.to_number();
            }

            bool operator<(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                return this->to_number() < other.to_number();
            }

            bool operator<=(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                return this->to_number() <= other.to_number();
            }

            float operator+(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                return this->to_number() + other.to_number();
            }

            float operator+=(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                auto value = this->to_number() + other.to_number();
                this->value = value;
                return value;
            }

            float operator-() {
                if (!this->is_number()) {
                    throw runtime_error("operand must be a number");
                }

                return -this->is_number();
            }

            float operator-(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                return this->to_number() - other.to_number();
            }

            float operator-=(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                auto value = this->to_number() - other.to_number();
                this->value = value;
                return value;
            }

            float operator*(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                return this->to_number() * other.to_number();
            }

            float operator*=(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                auto value = this->to_number() * other.to_number();
                this->value = value;
                return value;
            }

            float operator/(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                return this->to_number() / other.to_number();
            }

            float operator/=(Var& other) {
                if (!this->is_number() || !other.is_number()) {
                    throw runtime_error("both operands must be numbers");
                }

                auto value = this->to_number() / other.to_number();
                this->value = value;
                return value;
            }
    };
};

#endif
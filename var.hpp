#ifndef VAR_H
#define VAR_H

#include <any>
#include <string>

#include "error.hpp"

using namespace std;

namespace var {
    enum class Type {
        String,
        Number,
        Bool,
        Any
    };
    
    class Var {
        public:
            const Type type;
            any* value;

            ~Var() { delete this->value; }
            Var(any value) : type(Type::Any) { this->value = new any(value); }
            Var(string value) : type(Type::String) { this->value = new any(value); }
            Var(float value) : type(Type::Number) { this->value = new any(value); }
            Var(bool value) : type(Type::Bool) { this->value = new any(value); }

            void set(any value) { *this->value = value; }
            void set(string value) { *this->value = any(value); }
            void set(float value) { *this->value = any(value); }
            void set(bool value) { *this->value = any(value); }

            any get() { return *this->value; }
            string to_string() {
                if (this->type != Type::String) {
                    throw runtime_error("cannot convert type to string");
                }

                return any_cast<string>(*this->value);
            }

            float to_number() {
                if (this->type != Type::Number) {
                    throw runtime_error("cannot convert type to number");
                }

                return any_cast<float>(*this->value);
            }

            bool to_bool() {
                if (this->type != Type::Bool) {
                    throw runtime_error("cannot convert type to bool");
                }

                return any_cast<bool>(*this->value);
            }

            void nil() {
                delete this->value;
                this->value = NULL;
            }
    };
};

#endif
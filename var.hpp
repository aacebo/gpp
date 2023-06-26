#ifndef VAR_H
#define VAR_H

#include <any>
#include <string>

#include "error.hpp"

using namespace std;

namespace scope {   
    template <class T> 
    class Var {
        public:
            any value;

            Var(T value) { this->value = value; }

            void set(T value) { this->value = value; }
            T get() { return any_cast<T>(this->value); }

            void nil() { this->value.reset(); }
            bool is_nil() { return !this->value.has_value(); }
            string to_string() { return std::to_string(this->value); }

            bool operator==(Var<T>& other) {
                if (this->is_nil() && other.is_nil()) return true;
                return this->get() == other.get();
            }

            bool is_truthy() {
                if (this->is_nil()) return false;
                if (this->value.type() == typeid(bool)) {
                    return any_cast<bool>(this->value);
                }

                return true;
            }
    };
};

#endif
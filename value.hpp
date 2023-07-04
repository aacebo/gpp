#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <variant>

#include "value_type.hpp"
#include "object.hpp"
#include "string.hpp"

using namespace std;

namespace value {
    class Value {
        public:
            Type type;
            variant<bool, float, nullptr_t, Object*> value;

            Value();
            Value(bool);
            Value(float);
            Value(Object*);
            Value(const Value&);
            ~Value();

            bool is_bool();
            bool to_bool();

            bool is_number();
            float to_number();

            bool is_nil();
            nullptr_t to_nil();

            bool is_object();
            Object* to_object();

            bool is_string();
            String* to_string();

            string as_string();

            Value& operator=(const Value&);
            bool operator==(Value&);
            bool operator!=(Value&);
            bool operator>(Value&);
            bool operator>=(Value&);
            bool operator<(Value&);
            bool operator<=(Value&);
            float operator+(Value&);
            float operator+=(Value&);
            float operator-();
            float operator-(Value&);
            float operator-=(Value&);
            float operator*(Value&);
            float operator*=(Value&);
            float operator/(Value&);
            float operator/=(Value&);
    };
};

#endif

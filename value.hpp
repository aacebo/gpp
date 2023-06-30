#ifndef VALUE_H
#define VALUE_H

#include <variant>

using namespace std;

namespace value {
    class Object;

    enum class Type {
        Bool,
        Number,
        Nil,
        Object
    };

    class Value {
        variant<bool, float, nullptr_t, Object*> _value;

        public:
            const Type type;

            Value();
            Value(bool);
            Value(float);
            Value(Object*);

            bool is_bool();
            bool to_bool();

            bool is_number();
            float to_number();

            bool is_nil();
            nullptr_t to_nil();

            bool is_object();
            Object* to_object();
    };
};

#endif
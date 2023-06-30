#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <variant>

using namespace std;

namespace value {
    class Object;

    enum class Type {
        String,
        Bool,
        Number,
        Nil,
        Object
    };

    class Value {
        public:
            const Type type;
            variant<string, bool, float, nullptr_t, Object*> value;

            Value();
            Value(string);
            Value(bool);
            Value(float);
            Value(Object*);

            bool is_string();
            string to_string();

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

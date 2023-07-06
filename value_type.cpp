#include "value_type.hpp"

namespace value {
    string type_to_string(Type type) {
        switch (type) {
            case Type::Bool:
                return "bool";
            case Type::Nil:
                return "nil";
            case Type::Number:
                return "number";
            case Type::Object:
                return "object";
        }
    }
};

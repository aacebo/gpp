#include "expression_type.hpp"

namespace expression {
    string type_to_string(Type type) {
        switch (type) {
            case Type::Default:
                return "default";
            case Type::Assign:
                return "assign";
            case Type::Binary:
                return "binary";
            case Type::Call:
                return "call";
            case Type::Get:
                return "get";
            case Type::Set:
                return "set";
            case Type::Grouping:
                return "grouping";
            case Type::Literal:
                return "literal";
            case Type::Logical:
                return "logical";
            case Type::Super:
                return "super";
            case Type::Self:
                return "self";
            case Type::Unary:
                return "unary";
            case Type::Variable:
                return "variable";
        }
    }
};
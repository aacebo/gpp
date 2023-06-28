#include "statement_type.hpp"

namespace statement {
    string type_to_string(Type type) {
        switch (type) {
            case Type::Block:
                return "block";
            case Type::Function:
                return "function";
            case Type::Class:
                return "class";
            case Type::Expression:
                return "expression";
            case Type::If:
                return "if";
            case Type::Print:
                return "print";
            case Type::Return:
                return "return";
            case Type::Let:
                return "let";
            case Type::For:
                return "for";
            case Type::Use:
                return "use";
        }
    }
};

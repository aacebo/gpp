#include "precedence.hpp"

namespace compiler {
    Precedence get_precedence(parser::Type type) {
        switch (type) {
            case parser::Type::LParen:
            case parser::Type::Dot:
                return Precedence::Call;
            case parser::Type::Plus:
            case parser::Type::Minus:
                return Precedence::Term;
            case parser::Type::Star:
            case parser::Type::Slash:
                return Precedence::Factor;
            case parser::Type::EqEq:
            case parser::Type::NotEq:
                return Precedence::Equality;
            case parser::Type::Gt:
            case parser::Type::GtEq:
            case parser::Type::Lt:
            case parser::Type::LtEq:
                return Precedence::Comparison;
            case parser::Type::And:
                return Precedence::And;
            case parser::Type::Or:
                return Precedence::Or;
            default:
                return Precedence::None;
        }
    }
};

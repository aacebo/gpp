#include "precedence.hpp"

namespace compiler {
    Precedence get_precedence(parser::Type type) {
        switch (type) {
            case parser::Type::LParen:
                return Precedence::Call;
            case parser::Type::Dot:
                return Precedence::Call;
            case parser::Type::Plus:
                return Precedence::Term;
            case parser::Type::Minus:
                return Precedence::Term;
            case parser::Type::Star:
                return Precedence::Factor;
            case parser::Type::Slash:
                return Precedence::Factor;
            case parser::Type::NotEq:
                return Precedence::Equality;
            case parser::Type::EqEq:
                return Precedence::Equality;
            case parser::Type::Gt:
                return Precedence::Comparison;
            case parser::Type::GtEq:
                return Precedence::Comparison;
            case parser::Type::Lt:
                return Precedence::Comparison;
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

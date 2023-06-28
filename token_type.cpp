#include "token_type.hpp"

namespace token {
    Type keyword_to_type(string keyword) {
        if (keyword == "if") {
            return Type::If;
        } else if (keyword == "else") {
            return Type::Else;
        } else if (keyword == "for") {
            return Type::For;
        } else if (keyword == "let") {
            return Type::Let;
        } else if (keyword == "fn") {
            return Type::Fn;
        } else if (keyword == "return") {
            return Type::Return;
        } else if (keyword == "class") {
            return Type::Class;
        } else if (keyword == "super") {
            return Type::Super;
        } else if (keyword == "self") {
            return Type::Self;
        } else if (keyword == "pub") {
            return Type::Pub;
        } else if (keyword == "use") {
            return Type::Use;
        } else if (keyword == "true") {
            return Type::True;
        } else if (keyword == "false") {
            return Type::False;
        } else if (keyword == "print") {
            return Type::Print;
        }

        return Type::Invalid;
    }

    string type_to_string(Type type) {
        switch (type) {
            case Type::Comma:
                return ",";
            case Type::Dot:
                return ".";
            case Type::SemiColon:
                return ";";
            case Type::LParen:
                return "(";
            case Type::RParen:
                return ")";
            case Type::LBrace:
                return "{";
            case Type::RBrace:
                return "}";
            case Type::Minus:
                return "-";
            case Type::MinusEq:
                return "-=";
            case Type::Plus:
                return "+";
            case Type::PlusEq:
                return "+=";
            case Type::Slash:
                return "/";
            case Type::SlashEq:
                return "/=";
            case Type::Star:
                return "*";
            case Type::StarEq:
                return "*=";
            case Type::Not:
                return "!";
            case Type::NotEq:
                return "!=";
            case Type::Eq:
                return "=";
            case Type::EqEq:
                return "==";
            case Type::Gt:
                return ">";
            case Type::GtEq:
                return ">=";
            case Type::Lt:
                return "<";
            case Type::LtEq:
                return "<=";
            case Type::And:
                return "&&";
            case Type::Or:
                return "||";
            case Type::Identifier:
                return "identifier";
            case Type::String:
                return "string";
            case Type::Number:
                return "number";
            case Type::Nil:
                return "nil";
            case Type::If:
                return "if";
            case Type::Else:
                return "else";
            case Type::For:
                return "for";
            case Type::Let:
                return "let";
            case Type::Fn:
                return "fn";
            case Type::Return:
                return "return";
            case Type::Class:
                return "class";
            case Type::Super:
                return "super";
            case Type::Self:
                return "self";
            case Type::Pub:
                return "pub";
            case Type::Use:
                return "use";
            case Type::True:
                return "true";
            case Type::False:
                return "false";
            case Type::Print:
                return "print";
            case Type::Eof:
                return "eof";
            default:
                return "";
        }
    }
};
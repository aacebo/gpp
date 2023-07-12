#include "token.hpp"

namespace parser {
    Token::Token(
        Type type,
        int ln,
        int start,
        int end,
        string& value
    ) : type(type),
        ln(ln),
        start(start),
        end(end),
        value(value) {

    }

    Token::Token(
        const Token& token
    ) : type(token.type),
        ln(token.ln),
        start(token.start),
        end(token.end),
        value(token.value) {

    }

    const string Token::to_string() {
        return "[ln: " + std::to_string(this->ln) +
            ", start: " + std::to_string(this->start) +
            ", end: " + std::to_string(this->end) +
            "](" + type_to_string(this->type) + ") -> " + this->value;
    }

    const float Token::to_float() {
        return stof(this->value);
    }
};
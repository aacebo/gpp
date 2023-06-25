#include "token.hpp"
#include "error.hpp"

namespace token {
    Token::Token(
        Type type,
        int ln,
        int start,
        int end,
        string value
    ) : type(type), ln(ln), start(start), end(end), value(value) {

    }

    const string Token::fmt() {
        return "[ln: " + to_string(this->ln) +
            ", start: " + to_string(this->start) +
            ", end: " + to_string(this->end) +
            "](" + type_to_string(this->type) + ") -> " + this->value;
    }

    const float Token::to_float() {
        return stof(this->value);
    }
};
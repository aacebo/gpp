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

    const bool Token::to_bool() {
        if (this->value == "true") return true;
        else if (this->value == "false") return false;

        throw error::RuntimeError(
            this->ln,
            this->start,
            this->end,
            "string \"" + this->value + "\" cannot be cast to bool"
        );
    }
};
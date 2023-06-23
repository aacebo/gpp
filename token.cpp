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

    const string Token::to_string() {
        return type_to_string(this->type) + " -> " + this->value;
    }
};
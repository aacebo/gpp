#ifndef PRECEDENCE_H
#define PRECEDENCE_H

#include "token_type.hpp"

namespace compiler {
    enum class Precedence {
        None,
        Assignment, // =
        Or,         // ||
        And,        // &&
        Equality,   // ==, !=
        Comparison, // <, <=, >, >=
        Term,       // +, +=, -, -=
        Factor,     // *, *=, /, /=
        Unary,      // !, -
        Call,       // ., ()
        Primary
    };

    Precedence get_precedence(parser::Type);
};

#endif
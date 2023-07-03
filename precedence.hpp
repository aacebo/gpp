#ifndef PRECEDENCE_H
#define PRECEDENCE_H

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

    typedef void (*ParseFn)(bool canAssign);

    class Rule {
        const ParseFn prefix;
        const ParseFn infix;
        const Precedence precedence;

        public:
            Rule(ParseFn, ParseFn, Precedence);
    };
};

#endif
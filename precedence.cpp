#include "precedence.hpp"

namespace compiler {
    Rule::Rule(
        ParseFn prefix,
        ParseFn infix,
        Precedence precedence
    ) : prefix(prefix),
        infix(infix),
        precedence(precedence) {

    }
};

#include "interpreter.hpp"

namespace interpreter {
    any Interpreter::evaluate(expression::Expression expr) {
        return expr.accept(this);
    }

    void Interpreter::execute(statement::Statement stmt) {
        return stmt.accept(this);
    }
};

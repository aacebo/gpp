#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>

#include "visitor.hpp"
#include "scope.hpp"
#include "var.hpp"

using namespace std;

namespace interpreter {
    class Interpreter : public expression::Visitor<any>, public statement::Visitor<void> {
        const scope::Scope* _global = new scope::Scope();

        public:
            Interpreter() = default;
            ~Interpreter() { delete this->_global; }

        private:
            any evaluate(expression::Expression);
            void execute(statement::Statement);
    };
};

#endif
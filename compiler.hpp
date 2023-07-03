#ifndef COMPILER_H
#define COMPILER_H

#include <vector>
#include <string>
#include <unordered_map>

#include "function.hpp"
#include "parser.hpp"
#include "error.hpp"
#include "scanner.hpp"
#include "value.hpp"
#include "op_code.hpp"
#include "precedence.hpp"

using namespace std;

namespace compiler {
    class Compiler {
        value::Function* fn;
        parser::Parser* parser;
        unordered_map<string, value::Type> variables;
        vector<error::Error> errors;

        public:
            Compiler() = default;

            value::Function* compile(const string&);

        private:
            value::Type token_to_value_type(parser::Type);
            compiler::Rule get_token_rule(parser::Type);

            void _declaration();
            void _class();
            void _fn();
            void _let();

            void _statement();
            void _print();
            void _for();
            void _if();
            void _return();
            void _block();
            void _expression();
    };
};

#endif
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
#include "object.hpp"
#include "string.hpp"

using namespace std;

namespace compiler {
    class Compiler {
        Compiler* parent;
        value::Function* fn;
        parser::Parser* parser;
        unordered_map<string, value::Type> variables;
        vector<error::Error> errors;

        public:
            Compiler();
            Compiler(Compiler*);
            Compiler(string, Compiler*);

            vector<error::Error> get_errors();
            value::Function* compile(const string&);

        private:
            value::Type token_to_value_type(parser::Type);
            void expression();

            // emit helpers
            int jump(OpCode);
            void patch_jump(int);
            void loop(int);
            bool has_variable(string);

            // expressions
            // void _method();
            // void _function(value::FunctionType);

            // declarations
            void _declaration();
            // void _class();
            // void _fn();
            void _let();

            // statements
            void _statement();
            void _print();
            void _for();
            void _if();
            void _return();
            void _block();
            void _expression();

            // precedence
            void _precedence(Precedence);
            void _prefix(parser::Type, bool);
            void _infix(parser::Type, bool);

            // rules
            void _binary(bool);
            // void _call(bool);
            // void _dot(bool);
            void _literal(bool);
            void _grouping(bool);
            void _number(bool);
            void _and(bool);
            void _or(bool);
            void _string(bool);
            void _variable(bool);
            // void _super(bool);
            // void _self(bool);
            void _unary(bool);
    };
};

#endif
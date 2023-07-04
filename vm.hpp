#ifndef VM_H
#define VM_H

#include <stack>
#include <string>
#include <queue>
#include <iostream>

#include "compiler.hpp"
#include "value.hpp"
#include "string.hpp"
#include "frame.hpp"
#include "op_code.hpp"
#include "scope.hpp"

using namespace std;

namespace vm {
    class VM {
        value::Scope* scope;
        compiler::Compiler* compiler;
        queue<Frame*> frames;
        stack<value::Value> stack;

        public:
            VM();
            ~VM();

            void compile(const string&);
            void run();

        private:
            void _pop();
            void _const();
            void _nil();
            void _true();
            void _false();

            void _jump();
            void _jump_if_false();

            void _define();
            void _resolve();
            void _assign();

            void _add();
            void _subtract();
            void _multiply();
            void _divide();
            void _negate();

            void _not();
            void _eq();
            void _gt();
            void _lt();

            void _print();
    };
};

#endif

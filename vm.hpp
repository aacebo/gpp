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

using namespace std;

namespace vm {
    class VM {
        compiler::Compiler* compiler;
        queue<Frame*> frames;
        stack<value::Value> stack;

        public:
            VM();
            ~VM();

            void compile(const string&);
            void run();

        private:
            void _const();
            void _pop();
            void _add();
            void _subtract();
            void _multiply();
            void _divide();
            void _print();
    };
};

#endif

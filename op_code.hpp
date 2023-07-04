#ifndef OPCODE_H
#define OPCODE_H

#include <string>

using namespace std;

namespace compiler {
    enum class OpCode {
        Const,
        Nil,
        True,
        False,

        Pop,
        Define,
        Resolve,
        Assign,

        Eq,
        Gt,
        Lt,

        Add,
        Subtract,
        Multiply,
        Divide,

        Not,
        Negate,
        Print,
        Jump,
        JumpIfFalse,
        Loop,
        Call,
        Invoke,
        Closure,
        Return,
        Class,
        Inherit,
        Method
    };

    string code_to_string(OpCode);
};

#endif
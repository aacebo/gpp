#ifndef OPCODE_H
#define OPCODE_H

namespace compiler {
    enum class OpCode {
        Const,
        Nil,
        True,
        False,

        Pop,
        GetLocal,
        SetLocal,
        DefineGlobal,
        GetGlobal,
        SetGlobal,
        GetUpValue,
        SetUpValue,
        GetProperty,
        SetProperty,
        GetSuper,

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
        SuperInvoke,
        Closure,
        ClosureUpValue,
        Return,
        Class,
        Inherit,
        Method
    };
};

#endif
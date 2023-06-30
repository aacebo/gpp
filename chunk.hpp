#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

#include "value.hpp"

using namespace std;

namespace parser {
    enum class Type {
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

    class Operation {
        public:
            const uint8_t code;
            const int ln;

            Operation(uint8_t, int);
    };

    class Chunk {
        public:
            vector<Operation> ops;
            vector<value::Value> values;

            Chunk() = default;

            void push(uint8_t, int);
            int push(value::Value);
    };
};

#endif
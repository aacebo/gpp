#include "op_code.hpp"

namespace compiler {
    string code_to_string(OpCode code) {
        switch (code) {
            case OpCode::Const:
                return "const";
            case OpCode::Pop:
                return "pop";
            case OpCode::Define:
                return "define";
            case OpCode::Resolve:
                return "resolve";
            case OpCode::Assign:
                return "assign";
            case OpCode::Eq:
                return "equals";
            case OpCode::Gt:
                return "greater than";
            case OpCode::Lt:
                return "less than";
            case OpCode::Add:
                return "add";
            case OpCode::Subtract:
                return "subtract";
            case OpCode::Multiply:
                return "multiply";
            case OpCode::Divide:
                return "divide";
            case OpCode::Not:
                return "not";
            case OpCode::Negate:
                return "negate";
            case OpCode::Print:
                return "print";
            case OpCode::Jump:
                return "jump";
            case OpCode::JumpIfFalse:
                return "jump if false";
            case OpCode::Loop:
                return "loop";
            case OpCode::Call:
                return "call";
            case OpCode::Invoke:
                return "invoke";
            case OpCode::Closure:
                return "closure";
            case OpCode::Return:
                return "return";
            case OpCode::Class:
                return "class";
            case OpCode::Inherit:
                return "inherit";
            case OpCode::Method:
                return "method";
        }
    }
};

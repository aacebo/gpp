#include "vm.hpp"

namespace vm {
    VM::VM() {
        this->compiler = new compiler::Compiler();
    }

    VM::~VM() {
        delete this->compiler;
    }

    void VM::compile(const string& src) {
        auto fn = this->compiler->compile(src);
        this->frames.push(new Frame(dynamic_cast<value::Closure*>(fn)));
    }

    void VM::run() {
        while (!this->frames.empty()) {
            while (!this->frames.front()->is_empty()) {
                auto code = this->frames.front()->next_code();

                switch (code) {
                    case compiler::OpCode::Const:
                        this->_const();
                        break;
                    case compiler::OpCode::Add:
                        this->_add();
                        break;
                    case compiler::OpCode::Subtract:
                        this->_subtract();
                        break;
                    case compiler::OpCode::Multiply:
                        this->_multiply();
                        break;
                    case compiler::OpCode::Divide:
                        this->_divide();
                        break;
                    default:
                        // error unknown opcode
                }
            }

            delete this->frames.front();
            this->frames.pop();
        }
    }

    void VM::_const() {
        auto value = this->frames.front()->next_const();
        this->stack.push(value);
    }

    void VM::_add() {
        auto a = this->stack.top();
        this->stack.pop();
        auto b = this->stack.top();
        this->stack.pop();

        if (a.is_number()) {
            this->stack.push(value::Value(a + b));
        } else if (a.is_object() && a.is_string()) {
            this->stack.push(value::Value(*a.to_string() + *b.to_string()));
        }
    }

    void VM::_subtract() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
        this->stack.pop();
        this->stack.push(value::Value(a - b));
    }

    void VM::_multiply() {
        auto a = this->stack.top();
        this->stack.pop();
        auto b = this->stack.top();
        this->stack.pop();
        this->stack.push(value::Value(a * b));
    }

    void VM::_divide() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
        this->stack.pop();
        this->stack.push(value::Value(a / b));
    }
};

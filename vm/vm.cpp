#include "vm.hpp"

namespace vm {
    VM::VM() {
        this->scope = new value::Scope();
    }

    VM::~VM() {
        delete this->scope;
    }

    void VM::compile(const string& src) {
        auto compiler = new compiler::Compiler();
        auto fn = compiler->compile(src);
        auto errors = compiler->get_errors();
        delete compiler;

        if (errors.size() > 0) {
            throw errors;
        }

        this->frames.push(new Frame(fn));
    }

    void VM::run() {
        while (!this->frames.empty()) {
            while (!this->frames.front()->is_empty()) {
                auto code = this->frames.front()->next_code();
                // cout << "code: " << compiler::code_to_string(code) << endl;

                switch (code) {
                    case compiler::OpCode::Pop:
                        this->_pop();
                        break;
                    case compiler::OpCode::Const:
                        this->_const();
                        break;
                    case compiler::OpCode::Jump:
                        this->_jump();
                        break;
                    case compiler::OpCode::JumpIfFalse:
                        this->_jump_if_false();
                        break;
                    case compiler::OpCode::Loop:
                        this->_loop();
                        break;
                    case compiler::OpCode::Define:
                        this->_define();
                        break;
                    case compiler::OpCode::Resolve:
                        this->_resolve();
                        break;
                    case compiler::OpCode::Assign:
                        this->_assign();
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
                    case compiler::OpCode::Negate:
                        this->_negate();
                        break;
                    case compiler::OpCode::Not:
                        this->_not();
                        break;
                    case compiler::OpCode::Eq:
                        this->_eq();
                        break;
                    case compiler::OpCode::Gt:
                        this->_gt();
                        break;
                    case compiler::OpCode::Lt:
                        this->_lt();
                        break;
                    case compiler::OpCode::Print:
                        this->_print();
                        break;
                    case compiler::OpCode::Call:
                        this->_call();
                        break;
                    default:
                        break;
                }
            }

            // delete this->frames.front();
            this->frames.pop();
        }
    }

    void VM::_pop() {
        this->stack.pop();
    }

    void VM::_const() {
        auto value = this->frames.front()->next_const();
        this->stack.push(value);
    }

    void VM::_jump() {
        auto offset = this->frames.front()->next_short();
        this->frames.front()->jump_inc(offset);
    }

    void VM::_jump_if_false() {
        auto offset = this->frames.front()->next_short();
        
        if (!this->stack.top().is_truthy()) {
            this->frames.front()->jump_inc(offset);
        }
    }

    void VM::_loop() {
        auto offset = this->frames.front()->next_short();
        this->frames.front()->jump_dec(offset);
    }

    void VM::_define() {
        auto name = this->frames.front()->next_const();
        auto is_const = this->frames.front()->next_const();
        auto is_nilable = this->frames.front()->next_const();
        auto type = this->frames.front()->next_byte();
        auto value = this->stack.top();
        this->stack.pop();
        auto def = value::Definition(
            (value::Type)type,
            is_const.to_bool(),
            is_nilable.to_bool()
        );

        this->scope->define(name.to_string()->to_string(), def, value);
    }

    void VM::_resolve() {
        auto name = this->frames.front()->next_const();
        auto value = this->scope->resolve(name.to_string()->to_string());
        this->stack.push(value);
    }

    void VM::_assign() {
        auto name = this->frames.front()->next_const();
        auto value = this->stack.top();
        this->scope->assign(name.to_string()->to_string(), value);
    }

    void VM::_add() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
        this->stack.pop();

        if (a.is_number()) {
            this->stack.push(a + b);
        } else if (a.is_object() && a.is_string()) {
            this->stack.push(*a.to_string() + *b.to_string());
        }
    }

    void VM::_subtract() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
        this->stack.pop();
        this->stack.push(a - b);
    }

    void VM::_multiply() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
        this->stack.pop();
        this->stack.push(a * b);
    }

    void VM::_divide() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
        this->stack.pop();
        this->stack.push(a / b);
    }

    void VM::_negate() {
        auto value = this->stack.top();

        if (!value.is_number()) {
            throw runtime_error("cannot negate non number types");
        }

        this->stack.pop();
        this->stack.push(-value.to_number());
    }

    void VM::_not() {
        auto value = this->stack.top();
        this->stack.pop();
        this->stack.push(!value.is_truthy());
    }

    void VM::_eq() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
        this->stack.pop();
        this->stack.push(a == b);
    }

    void VM::_gt() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
        this->stack.pop();
        this->stack.push(a > b);
    }

    void VM::_lt() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
        this->stack.pop();
        this->stack.push(a < b);
    }

    void VM::_print() {
        auto value = this->stack.top();
        this->stack.pop();
        cout << value.as_string();
    }

    void VM::_call() {
        vector<value::Value> args;

        auto method = this->frames.front()->next_const();
        auto argc = this->frames.front()->next_const();

        for (int i = 0; i < argc.to_number(); i++) {
            args.push_back(this->stack.top());
            this->stack.pop();
        }

        auto value = this->stack.top();

        if (value.is_string()) {
            auto res = value.to_string()->call(method.to_string()->to_string());
            this->stack.push(value::Value::parse(res));
        }
    }
};

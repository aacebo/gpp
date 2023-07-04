#include "vm.hpp"

namespace vm {
    VM::VM() {
        this->compiler = new compiler::Compiler();
        this->scope = new value::Scope();
    }

    VM::~VM() {
        delete this->compiler;
        delete this->scope;
    }

    void VM::compile(const string& src) {
        auto fn = this->compiler->compile(src);
        auto errors = this->compiler->get_errors();

        if (errors.size() > 0) {
            for (auto e : errors) {
                cout << e.what() << endl;
            }

            return;
        }

        this->frames.push(new Frame(dynamic_cast<value::Closure*>(fn)));
    }

    void VM::run() {
        while (!this->frames.empty()) {
            while (!this->frames.front()->is_empty()) {
                auto code = this->frames.front()->next_code();
                cout << "code: " << compiler::code_to_string(code) << endl;

                switch (code) {
                    case compiler::OpCode::Const:
                        this->_const();
                        break;
                    case compiler::OpCode::Nil:
                        this->_nil();
                        break;
                    case compiler::OpCode::Negate:
                        this->_negate();
                        break;
                    case compiler::OpCode::True:
                        this->_true();
                        break;
                    case compiler::OpCode::False:
                        this->_false();
                        break;
                    case compiler::OpCode::Jump:
                        this->_jump();
                        break;
                    case compiler::OpCode::JumpIfFalse:
                        this->_jump_if_false();
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
                    case compiler::OpCode::Pop:
                        this->_pop();
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
                    case compiler::OpCode::Print:
                        this->_print();
                        break;
                    default:
                        break;
                }

                cout << endl;
            }

            delete this->frames.front();
            this->frames.pop();
        }
    }

    void VM::_const() {
        auto value = this->frames.front()->next_const();
        cout << "const: " << value.as_string() << endl;
        this->stack.push(value);
    }

    void VM::_nil() {
        cout << "nil" << endl;
        this->stack.push(value::Value());
    }

    void VM::_negate() {
        auto value = this->stack.top();

        if (!value.is_number()) {
            throw runtime_error("cannot negate non number types");
        }

        this->stack.pop();
        this->stack.push(value::Value(-value.to_number()));
    }

    void VM::_true() {
        this->stack.push(value::Value(true));
    }

    void VM::_false() {
        this->stack.push(value::Value(false));
    }

    void VM::_jump() {
        auto offset = this->frames.front()->next_short();
        this->frames.front()->jump_to(offset);
    }

    void VM::_jump_if_false() {
        auto offset = this->frames.front()->next_short();
        
        if (!this->stack.top().is_truthy()) {
            this->frames.front()->jump_to(offset);
        }
    }

    void VM::_define() {
        auto name = this->frames.front()->next_const();
        auto is_const = this->frames.front()->next_const();
        auto is_optional = this->frames.front()->next_const();
        auto type = this->frames.front()->next_byte();
        auto value = this->stack.top();
        this->stack.pop();
        auto def = value::Definition(
            (value::Type)type,
            is_const.to_bool(),
            is_optional.to_bool()
        );

        cout << "define: " << name.as_string() << " -> "
             << value.as_string() << " ("
             << value::type_to_string((value::Type)type);

        if (is_optional.to_bool()) {
            cout << "?";
        }

        if (is_const.to_bool()) {
            cout << " (const)";
        }

        cout << ")" << endl;

        this->scope->define(name.to_string()->to_string(), def, value);
    }

    void VM::_resolve() {
        auto name = this->frames.front()->next_const();
        auto value = this->scope->resolve(name.to_string()->to_string());
        cout << "resolve: " << name.as_string() << " -> " << value.as_string() << endl;
        this->stack.push(value);
    }

    void VM::_assign() {
        auto name = this->frames.front()->next_const();
        auto value = this->stack.top();
        cout << "assign: " << name.as_string() << " -> " << value.as_string() << endl;
        this->scope->assign(name.to_string()->to_string(), value);
    }

    void VM::_pop() {
        this->stack.pop();
    }

    void VM::_add() {
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
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
        auto b = this->stack.top();
        this->stack.pop();
        auto a = this->stack.top();
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

    void VM::_print() {
        auto value = this->stack.top();
        this->stack.pop();
        cout << value.as_string();
    }
};

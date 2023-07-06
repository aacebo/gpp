#include "compiler.hpp"

namespace compiler {
    Compiler::Compiler() {
        this->fn = new value::Closure();
    }

    Compiler::Compiler(Compiler* parent) : Compiler() {
        this->parent = parent;
        this->fn = new value::Closure();
    }

    Compiler::Compiler(string name, Compiler* parent) : Compiler(parent) {
        this->fn = new value::Function(name);
    }

    Compiler::~Compiler() {
        delete this->parser;
    }

    vector<error::Error> Compiler::get_errors() {
        vector<error::Error> errors;

        for (auto e : this->parser->errors) {
            errors.push_back(e);
        }

        for (auto e : this->errors) {
            errors.push_back(e);
        }

        return errors;
    }

    value::Function* Compiler::compile(const string& src) {
        this->parser = new parser::Parser(src);
        this->parser->next();

        while (!this->parser->match(parser::Type::Eof)) {
            this->_declaration();
        }

        return this->fn;
    }

    value::Type Compiler::token_to_value_type(parser::Type type) {
        switch (type) {
            case parser::Type::Number:
                return value::Type::Number;
            case parser::Type::Bool:
                return value::Type::Bool;
            case parser::Type::Nil:
                return value::Type::Nil;
            case parser::Type::String:
            case parser::Type::Identifier:
                return value::Type::Object;
            default:
                throw runtime_error("invalid token type");
        }
    }

    void Compiler::expression() {
        this->_precedence(Precedence::Assignment);
    }

    int Compiler::jump(OpCode code) {
        this->fn->chunk.push(code);
        this->fn->chunk.push(0xff);
        this->fn->chunk.push(0xff);
        return this->fn->chunk.size_bytes() - 2;
    }

    void Compiler::patch_jump(int offset) {
        int jump = this->fn->chunk.size_bytes() - offset - 2;
        this->fn->chunk.set(offset, (jump >> 8) & 0xff);
        this->fn->chunk.set(offset + 1, jump & 0xff);
    }

    void Compiler::loop(int start) {
        this->fn->chunk.push(OpCode::Loop);
        int offset = this->fn->chunk.size_bytes() - start + 2;
        this->fn->chunk.push((offset >> 8) & 0xff);
        this->fn->chunk.push(offset & 0xff);
    }

    bool Compiler::has_variable(string name) {
        if (this->variables.count(name) > 0) {
            return true;
        }

        if (this->parent) {
            return this->parent->has_variable(name);
        }

        return false;
    }

    void Compiler::_declaration() {
        if (this->parser->match(parser::Type::Class)) {
            // this->_class();
        } else if (this->parser->match(parser::Type::Fn)) {
            // this->_fn();
        } else if (this->parser->match(parser::Type::Let) || this->parser->match(parser::Type::Const)) {
            this->_let();
        } else {
            this->_statement();
        }

        if (this->parser->errors.size() > 0) {
            this->parser->sync();
        }
    }

    void Compiler::_let() {
        value::Type type = value::Type::Nil;
        bool is_const = this->parser->prev->type == parser::Type::Const;
        bool is_optional = false;

        this->parser->consume(parser::Type::Identifier, "expected variable name");
        auto name = *this->parser->prev;
        
        if (
            this->parser->match(parser::Type::Number) ||
            this->parser->match(parser::Type::Bool) ||
            this->parser->match(parser::Type::String) ||
            this->parser->match(parser::Type::Identifier)
        ) {
            type = this->token_to_value_type(this->parser->prev->type);

            if (this->parser->match(parser::Type::Optional)) {
                is_optional = true;
            }
        }

        if (this->parser->match(parser::Type::Eq)) {
            this->expression();
            auto init_type = this->fn->chunk.const_at(this->fn->chunk.size_consts() - 1).type;

            if (type != value::Type::Nil && type != init_type) {
                throw error::SyntaxError(
                    this->parser->prev->ln,
                    this->parser->prev->start,
                    this->parser->prev->end,
                    "cannot assign type '" + value::type_to_string(init_type) +
                    "' to variable of type '" + value::type_to_string(type) + "'"
                );
            }

            type = init_type;
        } else {
            this->fn->chunk.push(OpCode::Nil);
        }

        this->parser->consume(parser::Type::SemiColon, "expected ';' after variable declaration");
        this->variables[name.value] = type;
        auto name_value = dynamic_cast<value::Object*>(new value::String(name.value));
        this->fn->chunk.push(OpCode::Define);
        this->fn->chunk.push_const(value::Value(name_value));
        this->fn->chunk.push_const(value::Value(is_const));
        this->fn->chunk.push_const(value::Value(is_optional));
        this->fn->chunk.push((int)type);
    }

    void Compiler::_statement() {
        if (this->parser->match(parser::Type::Print)) {
            return this->_print();
        } else if (this->parser->match(parser::Type::For)) {
            return this->_for();
        } else if (this->parser->match(parser::Type::If)) {
            return this->_if();
        } else if (this->parser->match(parser::Type::Return)) {
            return this->_return();
        } else if (this->parser->match(parser::Type::LBrace)) {
            return this->_block();
        }

        return this->_expression();
    }

    void Compiler::_block() {
        while (
            this->parser->curr->type != parser::Type::RBrace &&
            this->parser->curr->type != parser::Type::Eof
        ) {
            this->_declaration();
        }

        this->parser->consume(parser::Type::RBrace, "expected '}' after block");
    }

    void Compiler::_expression() {
        this->expression();
        this->parser->consume(parser::Type::SemiColon, "expected ';' after expression");
        this->fn->chunk.push(OpCode::Pop);
    }

    void Compiler::_for() {
        this->parser->consume(parser::Type::LParen, "expected '(' after 'for'");

        // initializer
        if (this->parser->match(parser::Type::SemiColon)) {
            // do nothing
        } else if (
            this->parser->match(parser::Type::Let) ||
            this->parser->match(parser::Type::Const)
        ) {
            this->_let();
        } else {
            this->_expression();
        }

        int loop_start = this->fn->chunk.size_bytes();
        int jump_exit = -1;

        // exit condition
        if (!this->parser->match(parser::Type::SemiColon)) {
            this->expression();
            this->parser->consume(parser::Type::SemiColon, "expected ';' after loop condition");
            jump_exit = this->jump(OpCode::JumpIfFalse);
            this->fn->chunk.push(OpCode::Pop);
        }

        // incrementer
        if (!this->parser->match(parser::Type::RParen)) {
            int jump_body = this->jump(OpCode::Jump);
            int inc_start = this->fn->chunk.size_bytes();

            this->expression();
            this->fn->chunk.push(OpCode::Pop);
            this->parser->consume(parser::Type::RParen, "expected ')' after 'for' statement");
            
            this->loop(loop_start);
            loop_start = inc_start;
            this->patch_jump(jump_body);
        }

        this->_statement();
        this->loop(loop_start);

        if (jump_exit != -1) {
            this->patch_jump(jump_exit);
            this->fn->chunk.push(OpCode::Pop);
        }
    }

    void Compiler::_if() {
        this->parser->consume(parser::Type::LParen, "expected '(' after 'if'");
        this->expression();
        this->parser->consume(parser::Type::RParen, "expected ')' after condition");

        int then_jump = this->jump(OpCode::JumpIfFalse);
        this->fn->chunk.push(OpCode::Pop);
        this->_statement();

        int else_jump = this->jump(OpCode::Jump);
        this->patch_jump(then_jump);
        this->fn->chunk.push(OpCode::Pop);

        if (this->parser->match(parser::Type::Else)) {
            this->_statement();
        }

        this->patch_jump(else_jump);
    }

    void Compiler::_print() {
        this->expression();
        this->parser->consume(parser::Type::SemiColon, "expected ';' after value");
        this->fn->chunk.push(OpCode::Print);
    }

    void Compiler::_return() {
        if (this->parser->match(parser::Type::SemiColon)) {
            this->fn->chunk.push(OpCode::Nil);
            this->fn->chunk.push(0);
            this->fn->chunk.push(OpCode::Return);
            return;
        }

        this->expression();
        this->parser->consume(parser::Type::SemiColon, "expected ';' after return value");
        this->fn->chunk.push(OpCode::Return);
    }

    void Compiler::_precedence(Precedence precedence) {
        auto can_assign = precedence <= Precedence::Assignment;
        this->parser->next();

        try {
            this->_prefix(this->parser->prev->type, can_assign);
        } catch (runtime_error& e) {
            return this->errors.push_back(error::SyntaxError(
                this->parser->prev->ln,
                this->parser->prev->start,
                this->parser->prev->end,
                e.what()
            ));
        }

        while (precedence <= get_precedence(this->parser->curr->type)) {
            this->parser->next();
            this->_infix(this->parser->prev->type, can_assign);
        }

        if (can_assign && this->parser->match(parser::Type::Eq)) {
            return this->errors.push_back(error::SyntaxError(
                this->parser->prev->ln,
                this->parser->prev->start,
                this->parser->prev->end,
                "invalid assignment target"
            ));
        }
    }

    void Compiler::_prefix(parser::Type type, bool can_assign) {
        switch (type) {
            case parser::Type::LParen:
                return this->_grouping(can_assign);
            case parser::Type::Minus:
            case parser::Type::Not:
                return this->_unary(can_assign);
            case parser::Type::Identifier:
                return this->_variable(can_assign);
            case parser::Type::LString:
                return this->_string(can_assign);
            case parser::Type::LNumber:
                return this->_number(can_assign);
            case parser::Type::True:
            case parser::Type::False:
            case parser::Type::Nil:
                return this->_literal(can_assign);
            // case parser::Type::Super:
            //     return this->_super(can_assign);
            // case parser::Type::Self:
            //     return this->_self(can_assign);
            default:
                throw runtime_error("token type '" + parser::type_to_string(type) + "' is not supported");
        }
    }

    void Compiler::_infix(parser::Type type, bool can_assign) {
        switch (type) {
            // case parser::Type::LParen:
            //     return this->_call(can_assign);
            // case parser::Type::Dot:
            //     return this->_dot(can_assign);
            case parser::Type::And:
                return this->_and(can_assign);
            case parser::Type::Or:
                return this->_or(can_assign);
            case parser::Type::Minus:
            case parser::Type::Plus:
            case parser::Type::Slash:
            case parser::Type::Star:
            case parser::Type::EqEq:
            case parser::Type::NotEq:
            case parser::Type::Gt:
            case parser::Type::GtEq:
            case parser::Type::Lt:
            case parser::Type::LtEq:
                return this->_binary(can_assign);
            default:
                return;
        }
    }

    void Compiler::_binary(bool can_assign) {
        auto type = this->parser->prev->type;
        auto prec = get_precedence(type);
        this->_precedence((Precedence)((int)prec + 1));

        switch (type) {
            case parser::Type::EqEq:
                this->fn->chunk.push(OpCode::Eq);
                break;
            case parser::Type::NotEq:
                this->fn->chunk.push(OpCode::Eq);
                this->fn->chunk.push(OpCode::Not);
                break;
            case parser::Type::Gt:
                this->fn->chunk.push(OpCode::Gt);
                break;
            case parser::Type::GtEq:
                this->fn->chunk.push(OpCode::Lt);
                this->fn->chunk.push(OpCode::Not);
                break;
            case parser::Type::Lt:
                this->fn->chunk.push(OpCode::Lt);
                break;
            case parser::Type::LtEq:
                this->fn->chunk.push(OpCode::Gt);
                this->fn->chunk.push(OpCode::Not);
                break;
            case parser::Type::Plus:
                this->fn->chunk.push(OpCode::Add);
                break;
            case parser::Type::Minus:
                this->fn->chunk.push(OpCode::Subtract);
                break;
            case parser::Type::Star:
                this->fn->chunk.push(OpCode::Multiply);
                break;
            case parser::Type::Slash:
                this->fn->chunk.push(OpCode::Divide);
                break;
            default:
                throw runtime_error("invalid token type");
        }
    }

    void Compiler::_literal(bool can_assign) {
        switch (this->parser->prev->type) {
            case parser::Type::True:
                this->fn->chunk.push(OpCode::True);
                break;
            case parser::Type::False:
                this->fn->chunk.push(OpCode::False);
                break;
            case parser::Type::Nil:
                this->fn->chunk.push(OpCode::Nil);
                break;
            default:
                throw runtime_error("invalid token type");
        }
    }
    
    void Compiler::_grouping(bool can_assign) {
        this->expression();
        this->parser->consume(parser::Type::RParen, "expected ')' after expression");
    }

    void Compiler::_number(bool can_assign) {
        auto value = this->parser->prev->to_float();
        this->fn->chunk.push(OpCode::Const);
        this->fn->chunk.push_const(value::Value(value));
    }

    void Compiler::_and(bool can_assign) {
        int end_jump = this->jump(OpCode::JumpIfFalse);

        this->fn->chunk.push(OpCode::Pop);
        this->_precedence(Precedence::And);
        this->patch_jump(end_jump);
    }

    void Compiler::_or(bool can_assign) {
        int else_jump = this->jump(OpCode::JumpIfFalse);
        int end_jump = this->jump(OpCode::Jump);

        this->patch_jump(else_jump);
        this->fn->chunk.push(OpCode::Pop);
        this->_precedence(Precedence::Or);
        this->patch_jump(end_jump);
    }

    void Compiler::_string(bool can_assign) {
        auto value = dynamic_cast<value::Object*>(new value::String(this->parser->prev->value));
        this->fn->chunk.push(OpCode::Const);
        this->fn->chunk.push_const(value::Value(value));
    }

    void Compiler::_variable(bool can_assign) {
        auto name = *this->parser->prev;
        auto name_value = dynamic_cast<value::Object*>(new value::String(name.value));
        // this->parser->consume(parser::Type::SemiColon, "expected ';' after variable name");

        if (this->parser->match(parser::Type::Eq)) {
            this->expression();
            this->fn->chunk.push(OpCode::Assign);
        } else {
            this->fn->chunk.push(OpCode::Resolve);
        }

        this->fn->chunk.push_const(value::Value(name_value));
    }

    void Compiler::_unary(bool can_assign) {
        auto type = this->parser->prev->type;
        this->_precedence(Precedence::Unary);

        switch (type) {
            case parser::Type::Not:
                this->fn->chunk.push(OpCode::Not);
                break;
            case parser::Type::Minus:
                this->fn->chunk.push(OpCode::Negate);
                break;
            default:
                throw runtime_error("invalid token type");
        }
    }
};

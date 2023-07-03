#include "compiler.hpp"

namespace compiler {
    Compiler::Compiler() {
    }

    Compiler::Compiler(Compiler* parent) : Compiler() {
        this->parent = parent;
        this->fn = new value::Closure();
    }

    Compiler::Compiler(string name, Compiler* parent) : Compiler(parent) {
        this->fn = new value::Function(name);
    }

    value::Function* Compiler::compile(const string& src) {
        this->parser = new parser::Parser(src);

        while (this->parser->next()) {
            this->_declaration();
        }

        delete this->parser;
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

    void Compiler::_declaration() {
        if (this->parser->match(parser::Type::Class)) {
            this->_class();
        } else if (this->parser->match(parser::Type::Fn)) {
            this->_fn();
        } else if (this->parser->match(parser::Type::Let)) {
            this->_let();
        } else {
            this->_statement();
        }

        this->parser->sync();
    }

    void Compiler::_let() {
        parser::Type type = parser::Type::Nil;

        this->parser->consume(parser::Type::Identifier, "expected variable name");
        auto name = this->parser->prev;
        
        if (
            this->parser->match(parser::Type::Number) ||
            this->parser->match(parser::Type::Bool) ||
            this->parser->match(parser::Type::String) ||
            this->parser->match(parser::Type::Identifier)
        ) {
            type = this->parser->prev->type;
        }

        if (this->parser->match(parser::Type::Eq)) {
            this->_expression();
        } else {
            this->fn->chunk.push(OpCode::Nil);
        }

        this->parser->consume(parser::Type::SemiColon, "expected ';' after variable declaration");
        this->variables[name->value] = this->token_to_value_type(type);
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
            case parser::Type::Super:
                return this->_super(can_assign);
            case parser::Type::Self:
                return this->_self(can_assign);
            default:
                throw runtime_error("expected expression");
        }
    }

    void Compiler::_infix(parser::Type type, bool can_assign) {
        switch (type) {
            case parser::Type::LParen:
                return this->_call(can_assign);
            case parser::Type::Dot:
                return this->_dot(can_assign);
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
        }
    }
};

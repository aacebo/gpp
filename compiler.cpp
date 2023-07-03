#include "compiler.hpp"

namespace compiler {
    value::Function* Compiler::compile(const string& src) {
        this->fn = new value::Closure();
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

    compiler::Rule Compiler::get_token_rule(parser::Type type) {
        switch (type) {
            case parser::Type::LParen:
                return Rule(NULL, NULL, Precedence::Call);
            case parser::Type::RParen:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::LBrace:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::RBrace:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Comma:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Dot:
                return Rule(NULL, NULL, Precedence::Call);
            case parser::Type::Plus:
                return Rule(NULL, NULL, Precedence::Term);
            case parser::Type::Minus:
                return Rule(NULL, NULL, Precedence::Term);
            case parser::Type::Star:
                return Rule(NULL, NULL, Precedence::Factor);
            case parser::Type::Slash:
                return Rule(NULL, NULL, Precedence::Factor);
            case parser::Type::SemiColon:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Not:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Eq:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::NotEq:
                return Rule(NULL, NULL, Precedence::Equality);
            case parser::Type::EqEq:
                return Rule(NULL, NULL, Precedence::Equality);
            case parser::Type::Gt:
                return Rule(NULL, NULL, Precedence::Comparison);
            case parser::Type::GtEq:
                return Rule(NULL, NULL, Precedence::Comparison);
            case parser::Type::Lt:
                return Rule(NULL, NULL, Precedence::Comparison);
            case parser::Type::LtEq:
                return Rule(NULL, NULL, Precedence::Comparison);
            case parser::Type::Identifier:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::LString:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::LNumber:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::And:
                return Rule(NULL, NULL, Precedence::And);
            case parser::Type::Or:
                return Rule(NULL, NULL, Precedence::Or);
            case parser::Type::Class:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::If:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Else:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::True:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::False:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::For:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Fn:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Nil:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Print:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Return:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Super:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Self:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Let:
                return Rule(NULL, NULL, Precedence::None);
            case parser::Type::Eof:
                return Rule(NULL, NULL, Precedence::None);
            default:
                throw runtime_error("unsupported token type found while parsing");
        }
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
};

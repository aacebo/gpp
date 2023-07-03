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

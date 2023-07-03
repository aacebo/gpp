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

    value::Type Compiler::token_to_value_type(scanner::Type type) {
        switch (type) {
            case scanner::Type::Number:
                return value::Type::Number;
            case scanner::Type::Bool:
                return value::Type::Bool;
            case scanner::Type::Nil:
                return value::Type::Nil;
            case scanner::Type::String:
            case scanner::Type::Identifier:
                return value::Type::Object;
        }
    }

    void Compiler::_declaration() {
        if (this->parser->match(scanner::Type::Class)) {
            this->_class();
        } else if (this->parser->match(scanner::Type::Fn)) {
            this->_fn();
        } else if (this->parser->match(scanner::Type::Let)) {
            this->_let();
        } else {
            this->_statement();
        }

        this->parser->sync();
    }

    void Compiler::_let() {
        scanner::Type type = scanner::Type::Nil;

        this->parser->consume(scanner::Type::Identifier, "expected variable name");
        auto name = this->parser->prev;
        
        if (
            this->parser->match(scanner::Type::Number) ||
            this->parser->match(scanner::Type::Bool) ||
            this->parser->match(scanner::Type::String) ||
            this->parser->match(scanner::Type::Identifier)
        ) {
            type = this->parser->prev->type;
        }

        if (this->parser->match(scanner::Type::Eq)) {
            this->_expression();
        } else {
            this->fn->chunk.push(OpCode::Nil);
        }

        this->parser->consume(scanner::Type::SemiColon, "expected ';' after variable declaration");
        this->variables[name->value] = this->token_to_value_type(type);
    }
};

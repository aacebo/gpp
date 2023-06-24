#include "parser.hpp"

namespace parser {
    Parser::Parser(vector<token::Token> tokens) : _tokens(tokens) {

    }

    token::Token Parser::next() {
        if (!this->is_end()) {
            this->_it++;
        }

        return this->prev();
    }

    token::Token Parser::prev() {
        return this->_tokens[this->_it - 1];
    }

    token::Token Parser::peek() {
        return this->_tokens[this->_it];
    }

    const bool Parser::is_end() {
        return this->_tokens[this->_it].type == token::Type::Eof;
    }

    const bool Parser::is_type(token::Type type) {
        return this->peek().type == type;
    }

    const error::SyntaxError Parser::error(token::Token token, string message) {
        auto e = error::SyntaxError(
            token.ln,
            token.start,
            token.end,
            message
        );

        this->_errors.push_back(e);
        return e;
    }

    const bool Parser::match(initializer_list<token::Type> types) {
        for (auto type : types) {
            if (this->is_type(type)) {
                this->next();
                return true;
            }
        }

        return false;
    }

    const token::Token Parser::consume(token::Type type, string message) {
        if (this->is_type(type)) {
            return this->next();
        }

        throw this->error(this->peek(), message);
    }

    const expression::Expression Parser::_assignment() {
        auto expr = this->_or();

        if (this->match({token::Type::Eq})) {
            auto eq = this->prev();
            auto value = this->_assignment();

            if (auto e = static_cast<const expression::Variable*>(&expr)) {
                return expression::Assign(e->name, value);
            } else if (auto e = static_cast<const expression::Get*>(&expr)) {
                return expression::Set(e->object, e->name, value);
            }

            this->error(eq, "invalid assignment target");
        }

        return expr;
    }

    const expression::Expression Parser::_expression() {
        return this->_assignment();
    }

    const expression::Expression Parser::_primary() {
        if (this->match({token::Type::True})) return expression::Literal(true);
        if (this->match({token::Type::False})) return expression::Literal(false);
        if (this->match({token::Type::Nil})) return expression::Literal(nullptr);
        if (this->match({token::Type::Number})) return expression::Literal(this->prev().to_float());
        if (this->match({token::Type::String})) return expression::Literal(this->prev().value);
        if (this->match({token::Type::Self})) return expression::Self(this->prev());
        if (this->match({token::Type::Identifier})) return expression::Variable(this->prev());

        if (this->match({token::Type::Super})) {
            auto keyword = this->prev();
            this->consume(token::Type::Dot, "expected '.' after 'super'");
            return expression::Super(
                keyword,
                this->consume(token::Type::Identifier, "expected superclass method name")
            );
        }

        if (this->match({token::Type::LParen})) {
            auto expr = this->_expression();
            this->consume(token::Type::RParen, "expected ')' after expression");
            return expression::Grouping(expr);
        }

        throw this->error(this->peek(), "expected expression");
    }

    const expression::Expression Parser::_or() {
        auto expr = this->_and();

        while (this->match({token::Type::Or})) {
            return expression::Logical(
                expr,
                this->prev(),
                this->_and()
            );
        }

        return expr;
    }

    const expression::Expression Parser::_and() {
        auto expr = this->_equality();

        while (this->match({token::Type::And})) {
            return expression::Logical(
                expr,
                this->prev(),
                this->_equality()
            );
        }

        return expr;
    }

    const expression::Expression Parser::_equality() {
        auto expr = this->_comparison();

        while (this->match({token::Type::NotEq, token::Type::EqEq})) {
            return expression::Binary(
                expr,
                this->prev(),
                this->_comparison()
            );
        }

        return expr;
    }

    const expression::Expression Parser::_comparison() {
        auto expr = this->_term();

        while (this->match({token::Type::Gt, token::Type::GtEq, token::Type::Lt, token::Type::LtEq})) {
            return expression::Binary(
                expr,
                this->prev(),
                this->_term()
            );
        }

        return expr;
    }

    const expression::Expression Parser::_term() {
        auto expr = this->_factor();

        while (this->match({token::Type::Minus, token::Type::Plus})) {
            return expression::Binary(
                expr,
                this->prev(),
                this->_factor()
            );
        }

        return expr;
    }

    const expression::Expression Parser::_factor() {
        auto expr = this->_unary();

        while (this->match({token::Type::Slash, token::Type::Star})) {
            return expression::Binary(
                expr,
                this->prev(),
                this->_unary()
            );
        }

        return expr;
    }

    const expression::Expression Parser::_unary() {
        if (this->match({token::Type::Not, token::Type::Minus})) {
            return expression::Unary(
                this->prev(),
                this->_unary()
            );
        }

        return this->_call();
    }

    const expression::Expression Parser::_call() {
        auto expr = this->_primary();

        while (true) {
            if (this->match({token::Type::LParen})) {
                expr = this->_call_finish(expr);
            } else if (this->match({token::Type::Dot})) {
                expr = expression::Get(
                    expr,
                    this->consume(
                        token::Type::Identifier,
                        "expected property name after '.'"
                    )
                );
            } else {
                break;
            }
        }

        return expr;
    }

    const expression::Expression Parser::_call_finish(expression::Expression callee) {
        vector<expression::Expression> args;

        if (!this->is_type(token::Type::RParen)) {
            do {
                if (args.size() >= 255) {
                    this->error(this->peek(), "cannot have more than 255 arguments");
                }

                args.push_back(this->_expression());
            } while (this->match({token::Type::Comma}));
        }

        auto paren = this->consume(token::Type::RParen, "expected ')'");
        return expression::Call(callee, paren, args);
    }
};

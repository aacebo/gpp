#include "parser.hpp"

namespace parser {
    Parser::Parser(vector<token::Token> tokens) : _tokens(tokens) {
        vector<statement::Statement> stmts;

        while (!this->is_end()) {
            stmts.push_back(this->_declaration());
        }

        this->_statements = stmts;
    }

    const vector<statement::Statement> Parser::get_statements() {
        return this->_statements;
    }

    const vector<error::Error> Parser::get_errors() {
        return this->_errors;
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

    bool Parser::is_end() {
        return this->_tokens[this->_it].type == token::Type::Eof;
    }

    bool Parser::is_type(token::Type type) {
        return this->peek().type == type;
    }

    error::SyntaxError Parser::error(token::Token token, string message) {
        auto e = error::SyntaxError(
            token.ln,
            token.start,
            token.end,
            message
        );

        this->_errors.push_back(e);
        return e;
    }

    bool Parser::match(initializer_list<token::Type> types) {
        for (auto type : types) {
            if (this->is_type(type)) {
                this->next();
                return true;
            }
        }

        return false;
    }

    token::Token Parser::consume(token::Type type, string message) {
        if (this->is_type(type)) {
            return this->next();
        }

        throw this->error(this->peek(), message);
    }

    void Parser::sync() {
        this->next();

        while (!this->is_end()) {
            if (this->prev().type == token::Type::SemiColon) return;

            switch (this->peek().type) {
                case token::Type::Class:
                case token::Type::Fn:
                case token::Type::Let:
                case token::Type::For:
                case token::Type::If:
                case token::Type::Return:
                case token::Type::Use:
                    return;
                default: break;
            }

            this->next();
        }
    }

    expression::Expression Parser::_assignment() {
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

    expression::Expression Parser::_expression() {
        return this->_assignment();
    }

    expression::Expression Parser::_primary() {
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

        throw this->error(this->peek(), "expected expression \"" + token::type_to_string(this->peek().type) + "\"");
    }

    expression::Expression Parser::_or() {
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

    expression::Expression Parser::_and() {
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

    expression::Expression Parser::_equality() {
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

    expression::Expression Parser::_comparison() {
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

    expression::Expression Parser::_term() {
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

    expression::Expression Parser::_factor() {
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

    expression::Expression Parser::_unary() {
        if (this->match({token::Type::Not, token::Type::Minus})) {
            return expression::Unary(
                this->prev(),
                this->_unary()
            );
        }

        return this->_call();
    }

    expression::Expression Parser::_call() {
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

    expression::Expression Parser::_call_finish(expression::Expression callee) {
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

    statement::Statement Parser::_declaration() {
        try {
            if (this->match({token::Type::Class})) return this->_class();
            if (this->match({token::Type::Fn})) return this->_function("function");
            if (this->match({token::Type::Let})) return this->_var();
            if (this->match({token::Type::Use})) return this->_use();
        } catch (error::SyntaxError err) {
            this->sync();
        }

        return this->_statement();
    }

    statement::Statement Parser::_class() {
        expression::Variable* superclass;
        auto name = this->consume(token::Type::Identifier, "expected class name");

        if (this->match({token::Type::Lt})) {
            this->consume(token::Type::Identifier, "expected superclass name");
            superclass = new expression::Variable(this->prev());
        }

        this->consume(token::Type::LBrace, "expected '{' before class body");

        vector<statement::Function> methods;

        while (!this->is_type(token::Type::RBrace) && !this->is_end()) {
            methods.push_back(this->_function("method"));
        }

        this->consume(token::Type::RBrace, "expected '}' after class body");
        return statement::Class(name, superclass, methods);
    }

    statement::Statement Parser::_statement() {
        if (this->match({token::Type::For})) return this->_for();
        if (this->match({token::Type::If})) return this->_if();
        if (this->match({token::Type::Return})) return this->_return();
        if (this->match({token::Type::LBrace})) return statement::Block(this->_block());
        return this->_expr();
    }

    statement::Statement Parser::_for() {
        statement::Statement* init;
        this->consume(token::Type::LParen, "expected '(' after 'for'");

        if (this->match({token::Type::SemiColon})) {
            init = NULL;
        } else if (this->match({token::Type::Let})) {
            init = new statement::Statement(this->_var());
        } else {
            init = new statement::Statement(this->_expr());
        }

        expression::Expression* condition;

        if (!this->is_type(token::Type::SemiColon)) {
            condition = new expression::Expression(this->_expression());
        }

        this->consume(token::Type::SemiColon, "expected ';' after loop");
        expression::Expression* increment;

        if (!this->is_type(token::Type::RParen)) {
            increment = new expression::Expression(this->_expression());
        }

        this->consume(token::Type::RParen, "expected ')' after 'for' clause");
        auto body = new statement::Statement(this->_statement());

        if (increment != NULL) {
            vector<statement::Statement> stmts{*body, statement::Expression(*increment)};
            body = new statement::Block(stmts);
        }

        if (condition == NULL) {
            condition = new expression::Literal(true);
        }

        body = new statement::For(*condition, *body);

        if (init != NULL) {
            vector<statement::Statement> stmts{*init, *body};
            body = new statement::Block(stmts);
        }

        return *body;
    }

    statement::Statement Parser::_if() {
        this->consume(token::Type::LParen, "expected '(' after 'if'");
        auto condition = this->_expression();
        this->consume(token::Type::RParen, "expected ')' after 'if' condition");

        auto then_branch = new statement::Statement(this->_statement());
        statement::Statement* else_branch;

        if (this->match({token::Type::Else})) {
            else_branch = new statement::Statement(this->_statement());
        }

        return statement::If(condition, then_branch, else_branch);
    }

    statement::Statement Parser::_return() {
        auto keyword = this->prev();
        expression::Expression* value;

        if (!this->is_type(token::Type::SemiColon)) {
            value = new expression::Expression(this->_expression());
        }

        this->consume(token::Type::SemiColon, "expected ';' after return value");
        return statement::Return(keyword, value);
    }

    statement::Statement Parser::_var() {
        expression::Expression* init;
        auto name = this->consume(token::Type::Identifier, "expected variable name");

        if (this->match({token::Type::Eq})) {
            init = new expression::Expression(this->_expression());
        }

        this->consume(token::Type::SemiColon, "expected ';' after variable declaration");
        return statement::Let(name, init);
    }

    statement::Statement Parser::_expr() {
        auto expr = this->_expression();
        this->consume(token::Type::SemiColon, "expected ';' after expression");
        return statement::Expression(expr);
    }

    statement::Statement Parser::_use() {
        auto name = this->consume(token::Type::Identifier, "expected package name");
        this->consume(token::Type::SemiColon, "expected ';' after package import");
        return statement::Use(name);
    }

    statement::Function Parser::_function(string kind) {
        vector<token::Token> params;
        auto name = this->consume(token::Type::Identifier, "expected \"" + kind + "\" name");
        this->consume(token::Type::LParen, "expected '(' after \"" + kind + "\" name");

        if (!this->is_type(token::Type::RParen)) {
            do {
                if (params.size() >= 255) {
                    this->error(this->peek(), "cannot have more than 255 parameters");
                }

                params.push_back(this->consume(token::Type::Identifier, "expected parameter name"));
            } while (this->match({token::Type::Comma}));
        }

        this->consume(token::Type::RParen, "expected ')' after parameters");
        this->consume(token::Type::LBrace, "expected '{' before \"" + kind + "\" body");
        return statement::Function(name, params, this->_block());
    }

    vector<statement::Statement> Parser::_block() {
        vector<statement::Statement> stmts;

        while (!this->is_type(token::Type::RBrace) && !this->is_end()) {
            stmts.push_back(this->_declaration());
        }

        this->consume(token::Type::RBrace, "expected '}' after block");
        return stmts;
    }
};

#include "parser.hpp"

namespace parser {
    Parser::Parser(const string& src) {
        this->scanner = new scanner::Scanner(src);
    }

    Parser::~Parser() {
        delete this->scanner;
        delete this->curr;
        delete this->prev;
    }

    Rule::Rule(
        ParseFn prefix,
        ParseFn infix,
        Precedence precedence
    ) : prefix(prefix),
        infix(infix),
        precedence(precedence) {

    }

    bool Parser::next() {
        delete this->prev;
        this->prev = this->curr;

        for (;;) {
            try {
                this->curr = this->scanner->scan();

                if (this->curr->type == scanner::Type::Eof) {
                    return false;
                }

                return true;
            } catch (error::Error& e) {
                this->errors.push_back(e);
            }
        }
    }

    bool Parser::match(scanner::Type type) {
        if (this->curr->type != type) return false;
        this->next();
        return true;
    }

    void Parser::consume(scanner::Type type, const string& message) {
        if (this->curr->type == type) {
            this->next();
            return;
        }

        this->errors.push_back(error::SyntaxError(
            this->curr->ln,
            this->curr->start,
            this->curr->end,
            message
        ));
    }

    void Parser::sync() {
        while (this->curr->type != scanner::Type::Eof) {
            if (this->prev->type == scanner::Type::SemiColon) {
                return;
            }

            switch (this->curr->type) {
                case scanner::Type::Class:
                case scanner::Type::Fn:
                case scanner::Type::Let:
                case scanner::Type::For:
                case scanner::Type::If:
                case scanner::Type::Print:
                case scanner::Type::Return:
                    return;
                default:
            }

            this->next();
        }
    }

    Rule Parser::get_token_rule(scanner::Type type) {
        switch (type) {
            case scanner::Type::LParen:
                return Rule(NULL, NULL, Precedence::Call);
            case scanner::Type::RParen:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::LBrace:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::RBrace:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Comma:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Dot:
                return Rule(NULL, NULL, Precedence::Call);
            case scanner::Type::Plus:
                return Rule(NULL, NULL, Precedence::Term);
            case scanner::Type::Minus:
                return Rule(NULL, NULL, Precedence::Term);
            case scanner::Type::Star:
                return Rule(NULL, NULL, Precedence::Factor);
            case scanner::Type::Slash:
                return Rule(NULL, NULL, Precedence::Factor);
            case scanner::Type::SemiColon:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Not:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Eq:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::NotEq:
                return Rule(NULL, NULL, Precedence::Equality);
            case scanner::Type::EqEq:
                return Rule(NULL, NULL, Precedence::Equality);
            case scanner::Type::Gt:
                return Rule(NULL, NULL, Precedence::Comparison);
            case scanner::Type::GtEq:
                return Rule(NULL, NULL, Precedence::Comparison);
            case scanner::Type::Lt:
                return Rule(NULL, NULL, Precedence::Comparison);
            case scanner::Type::LtEq:
                return Rule(NULL, NULL, Precedence::Comparison);
            case scanner::Type::Identifier:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::LString:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::LNumber:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::And:
                return Rule(NULL, NULL, Precedence::And);
            case scanner::Type::Or:
                return Rule(NULL, NULL, Precedence::Or);
            case scanner::Type::Class:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::If:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Else:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::True:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::False:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::For:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Fn:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Nil:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Print:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Return:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Super:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Self:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Let:
                return Rule(NULL, NULL, Precedence::None);
            case scanner::Type::Eof:
                return Rule(NULL, NULL, Precedence::None);
            default:
                throw runtime_error("unsupported token type found while parsing");
        }
    }
};

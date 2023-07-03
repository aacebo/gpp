#include "parser.hpp"

namespace parser {
    Parser::Parser(const string& src) {
        this->scanner = new Scanner(src);
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

                if (this->curr->type == Type::Eof) {
                    return false;
                }

                return true;
            } catch (error::Error& e) {
                this->errors.push_back(e);
            }
        }
    }

    bool Parser::match(Type type) {
        if (this->curr->type != type) return false;
        this->next();
        return true;
    }

    void Parser::consume(Type type, const string& message) {
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
        while (this->curr->type != Type::Eof) {
            if (this->prev->type == Type::SemiColon) {
                return;
            }

            switch (this->curr->type) {
                case Type::Class:
                case Type::Fn:
                case Type::Let:
                case Type::For:
                case Type::If:
                case Type::Print:
                case Type::Return:
                    return;
                default:
            }

            this->next();
        }
    }

    Rule Parser::get_token_rule(Type type) {
        switch (type) {
            case Type::LParen:
                return Rule(NULL, NULL, Precedence::Call);
            case Type::RParen:
                return Rule(NULL, NULL, Precedence::None);
            case Type::LBrace:
                return Rule(NULL, NULL, Precedence::None);
            case Type::RBrace:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Comma:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Dot:
                return Rule(NULL, NULL, Precedence::Call);
            case Type::Plus:
                return Rule(NULL, NULL, Precedence::Term);
            case Type::Minus:
                return Rule(NULL, NULL, Precedence::Term);
            case Type::Star:
                return Rule(NULL, NULL, Precedence::Factor);
            case Type::Slash:
                return Rule(NULL, NULL, Precedence::Factor);
            case Type::SemiColon:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Not:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Eq:
                return Rule(NULL, NULL, Precedence::None);
            case Type::NotEq:
                return Rule(NULL, NULL, Precedence::Equality);
            case Type::EqEq:
                return Rule(NULL, NULL, Precedence::Equality);
            case Type::Gt:
                return Rule(NULL, NULL, Precedence::Comparison);
            case Type::GtEq:
                return Rule(NULL, NULL, Precedence::Comparison);
            case Type::Lt:
                return Rule(NULL, NULL, Precedence::Comparison);
            case Type::LtEq:
                return Rule(NULL, NULL, Precedence::Comparison);
            case Type::Identifier:
                return Rule(NULL, NULL, Precedence::None);
            case Type::LString:
                return Rule(NULL, NULL, Precedence::None);
            case Type::LNumber:
                return Rule(NULL, NULL, Precedence::None);
            case Type::And:
                return Rule(NULL, NULL, Precedence::And);
            case Type::Or:
                return Rule(NULL, NULL, Precedence::Or);
            case Type::Class:
                return Rule(NULL, NULL, Precedence::None);
            case Type::If:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Else:
                return Rule(NULL, NULL, Precedence::None);
            case Type::True:
                return Rule(NULL, NULL, Precedence::None);
            case Type::False:
                return Rule(NULL, NULL, Precedence::None);
            case Type::For:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Fn:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Nil:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Print:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Return:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Super:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Self:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Let:
                return Rule(NULL, NULL, Precedence::None);
            case Type::Eof:
                return Rule(NULL, NULL, Precedence::None);
            default:
                throw runtime_error("unsupported token type found while parsing");
        }
    }
};

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
                    break;
            }

            this->next();
        }
    }
};

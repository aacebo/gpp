#include "parser.hpp"

namespace parser {
    Parser::Parser(string src) {
        this->scanner = new scanner::Scanner(src);
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

    void Parser::consume(scanner::Type type, string message) {
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
};

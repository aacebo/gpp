#include "scanner.hpp"
#include "error.hpp"

namespace scanner {
    Scanner::Scanner(string src) : _src(src) {
        this->_left = 0;
        this->_right = 0;
        this->_ln = 0;
        this->scan();
    }

    const vector<token::Token> Scanner::get_tokens() {
        return this->_tokens;
    }

    const vector<error::Error> Scanner::get_errors() {
        return this->_errors;
    }

    void Scanner::scan() {
        while (this->_right < this->_src.length()) {
            this->_left = this->_right;
            char c = this->_src[_right];
            this->_right++;

            switch (c) {
                case ' ':
                case '\r':
                case '\t':
                    // ignore whitespace
                    break;
                case '\n':
                    this->_ln++;
                    break;
                case '(':
                    this->push(token::Type::LParen);
                    break;
                case ')':
                    this->push(token::Type::RParen);
                    break;
                case '{':
                    this->push(token::Type::LBrace);
                    break;
                case '}':
                    this->push(token::Type::RBrace);
                    break;
                case ',':
                    this->push(token::Type::Comma);
                    break;
                case '.':
                    this->push(token::Type::Dot);
                    break;
                case ';':
                    this->push(token::Type::SemiColon);
                    break;
                case '-':
                    this->push(this->peek() == '=' ? token::Type::MinusEq : token::Type::Minus);
                    break;
                case '+':
                    this->push(this->peek() == '=' ? token::Type::PlusEq : token::Type::Plus);
                    break;
                case '*':
                    this->push(this->peek() == '=' ? token::Type::StarEq : token::Type::Star);
                    break;
                case '/':
                    if (this->peek() == '/') {
                        this->on_comment();
                    } else if (this->peek() == '=') {
                        this->push(token::Type::SlashEq);
                        this->_right++;
                    } else {
                        this->push(token::Type::Slash);
                    }

                    break;
                case '"':
                    this->on_string();
                    break;
                case '!':
                    this->push(this->peek() == '=' ? token::Type::NotEq : token::Type::Not);
                    break;
                case '=':
                    this->push(this->peek() == '=' ? token::Type::EqEq : token::Type::Eq);
                    break;
                case '<':
                    this->push(this->peek() == '=' ? token::Type::LtEq : token::Type::Lt);
                    break;
                case '>':
                    this->push(this->peek() == '=' ? token::Type::GtEq : token::Type::Gt);
                    break;
                default:
                    if (this->is_integer(c)) {
                        this->on_number();
                        break;
                    } else if (this->is_alpha(c)) {
                        this->on_identifier();
                        break;
                    }

                    this->_errors.push_back(error::SyntaxError(
                        this->_ln,
                        this->_left,
                        this->_right,
                        "unexpected character"
                    ));
            }
        }

        this->push(token::Type::Eof);
    }

    void Scanner::push(token::Type type) {
        this->_tokens.push_back(token::Token(
            type,
            this->_ln,
            this->_left,
            this->_right,
            this->_src.substr(this->_left, this->_right - this->_left)
        ));
    }

    char Scanner::peek() {
        if (this->_right >= this->_src.length()) {
            return '\0';
        }

        return this->_src[this->_right];
    }

    bool Scanner::is_integer(char c) {
        return c >= '0' && c <= '9';
    }

    bool Scanner::is_alpha(char c) {
        return (
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_'
        );
    }

    void Scanner::on_comment() {
        while (this->peek() != '\n' && this->peek() != '\0') {
            this->_right++;
        }

        this->_ln++;
        this->_right++;
    }

    void Scanner::on_string() {
        while (this->peek() != '"' && this->peek() != '\0') {
            if (this->peek() == '\n') {
                this->_ln++;
            }

            this->_right++;
        }

        if (this->_right == this->_src.length()) {
            this->_errors.push_back(error::SyntaxError(
                this->_ln,
                this->_left,
                this->_right,
                "unterminated string"
            ));

            return;
        }

        this->_right++;
        this->push(token::Type::String);
    }

    void Scanner::on_number() {
        while (this->is_integer(this->peek())) {
            this->_right++;
        }

        if (this->peek() == '.') {
            this->_right++;

            while (this->is_integer(this->peek())) {
                this->_right++;
            }
        }

        this->push(token::Type::Number);
    }

    void Scanner::on_identifier() {
        while (this->is_alpha(this->peek()) || this->is_integer(this->peek())) {
            this->_right++;
        }

        const string identifier = this->_src.substr(
            this->_left,
            this->_right - this->_left
        );

        if (token::keyword_to_type(identifier) != token::Type::Invalid) {
            this->push(token::keyword_to_type(identifier));
        } else {
            this->push(token::Type::Identifier);
        }
    }
};

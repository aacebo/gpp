#include "scanner.hpp"
#include "error.hpp"

namespace scanner {
    Scanner::Scanner(string src) : _src(src) {
        this->_left = 0;
        this->_right = 0;
        this->_ln = 0;
    }

    token::Token Scanner::scan() {
        if (this->_right == this->_src.length() - 1) {
            return this->create(token::Type::Eof);
        }

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
                return this->create(token::Type::LParen);
            case ')':
                return this->create(token::Type::RParen);
            case '{':
                return this->create(token::Type::LBrace);
            case '}':
                return this->create(token::Type::RBrace);
            case ',':
                return this->create(token::Type::Comma);
            case '.':
                return this->create(token::Type::Dot);
            case ';':
                return this->create(token::Type::SemiColon);
            case '?':
                return this->create(token::Type::Optional);
            case '-':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(token::Type::MinusEq);
                } else if (this->peek() == '>') {
                    this->_right++;
                    return this->create(token::Type::ReturnType);
                }

                return this->create(token::Type::Minus);
            case '+':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(token::Type::PlusEq);
                }

                return this->create(token::Type::Plus);
            case '*':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(token::Type::StarEq);
                }

                return this->create(token::Type::Star);
            case '/':
                if (this->peek() == '/') {
                    return this->on_comment();
                } else if (this->peek() == '=') {
                    this->_right++;
                    return this->create(token::Type::SlashEq);
                }

                return this->create(token::Type::Slash);
            case '!':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(token::Type::NotEq);
                }

                return this->create(token::Type::Not);
            case '=':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(token::Type::EqEq);
                }

                return this->create(token::Type::Eq);
            case '<':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(token::Type::LtEq);
                }

                return this->create(token::Type::Lt);
            case '>':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(token::Type::GtEq);
                }

                return this->create(token::Type::Gt);
            case '"':
                return this->on_string();
            default:
                if (this->is_integer(c)) {
                    return this->on_number();
                } else if (this->is_alpha(c)) {
                    return this->on_identifier();
                }

                throw error::SyntaxError(
                    this->_ln,
                    this->_left,
                    this->_right,
                    "unexpected character"
                );
        }

        return this->scan();
    }

    token::Token Scanner::create(token::Type type) {
        auto value = this->_src.substr(this->_left, this->_right - this->_left);

        // handle escaped characters
        if (type == token::Type::LString) {
            for (int i = 0; i < value.length() - 1; i++) {
                if (value[i] == '\\') {
                    value.erase(i, 1);
                    
                    switch (value[i]) {
                        case 'a': // bell (alert)
                            value[i] = '\a';
                            break;
                        case 'b': // backspace
                            value[i] = '\b';
                            break;
                        case 't': // horizontal tab
                            value[i] = '\t';
                            break;
                        case 'v': // vertical tab
                            value[i] = '\v';
                            break;
                        case 'n': // new line
                            value[i] = '\n';
                            break;
                        case 'f': // form feed
                            value[i] = '\f';
                            break;
                        case 'r': // carriage return
                            value[i] = '\r';
                            break;
                        case '\'': // single quotation
                            value[i] = '\'';
                            break;
                        case '"': // double quotation
                            value[i] = '\"';
                            break;
                    }

                    i--;
                }
            }
        }

        return token::Token(
            type,
            this->_ln,
            this->_left / this->_ln,
            (this->_left / this->_ln) + (this->_right - this->_left),
            value
        );
    }

    char Scanner::peek() {
        if (this->_right >= this->_src.length()) {
            return '\0';
        }

        return this->_src[this->_right];
    }

    bool Scanner::is_escaped() {
        int i = this->_right - 1;
        bool escaped = false;

        while (i > -1 && this->_src[i] == '\\') {
            escaped = !escaped;
            i--;
        }

        return escaped;
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

    token::Token Scanner::on_comment() {
        while (this->peek() != '\n' && this->peek() != '\0') {
            this->_right++;
        }

        this->_ln++;
        this->_right++;
        return this->scan();
    }

    token::Token Scanner::on_string() {
        while ((this->peek() != '"' || this->is_escaped()) && this->peek() != '\0') {
            if (this->peek() == '\n') {
                this->_ln++;
            }

            this->_right++;
        }

        if (this->_right == this->_src.length()) {
            throw error::SyntaxError(
                this->_ln,
                this->_left,
                this->_right,
                "unterminated string"
            );
        }

        this->_left++;
        auto token = this->create(token::Type::LString);
        this->_right++;
        return token;
    }

    token::Token Scanner::on_number() {
        while (this->is_integer(this->peek())) {
            this->_right++;
        }

        if (this->peek() == '.') {
            this->_right++;

            while (this->is_integer(this->peek())) {
                this->_right++;
            }
        }

        return this->create(token::Type::LNumber);
    }

    token::Token Scanner::on_identifier() {
        while (this->is_alpha(this->peek()) || this->is_integer(this->peek())) {
            this->_right++;
        }

        const string identifier = this->_src.substr(
            this->_left,
            this->_right - this->_left
        );

        if (token::keyword_to_type(identifier) != token::Type::Invalid) {
            return this->create(token::keyword_to_type(identifier));
        }

        return this->create(token::Type::Identifier);
    }
};

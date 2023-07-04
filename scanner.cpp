#include "scanner.hpp"

namespace parser {
    Scanner::Scanner(const string& src) : _src(src) {
        this->_left = 0;
        this->_right = 0;
        this->_ln = 0;
    }

    Token* Scanner::scan() {
        if (this->_right == this->_src.length()) {
            return this->create(Type::Eof);
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
                return this->create(Type::LParen);
            case ')':
                return this->create(Type::RParen);
            case '{':
                return this->create(Type::LBrace);
            case '}':
                return this->create(Type::RBrace);
            case ',':
                return this->create(Type::Comma);
            case '.':
                return this->create(Type::Dot);
            case ';':
                return this->create(Type::SemiColon);
            case '?':
                return this->create(Type::Optional);
            case '-':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(Type::MinusEq);
                } else if (this->peek() == '>') {
                    this->_right++;
                    return this->create(Type::ReturnType);
                }

                return this->create(Type::Minus);
            case '+':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(Type::PlusEq);
                }

                return this->create(Type::Plus);
            case '*':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(Type::StarEq);
                }

                return this->create(Type::Star);
            case '/':
                if (this->peek() == '/') {
                    return this->on_comment();
                } else if (this->peek() == '=') {
                    this->_right++;
                    return this->create(Type::SlashEq);
                }

                return this->create(Type::Slash);
            case '!':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(Type::NotEq);
                }

                return this->create(Type::Not);
            case '=':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(Type::EqEq);
                }

                return this->create(Type::Eq);
            case '<':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(Type::LtEq);
                }

                return this->create(Type::Lt);
            case '>':
                if (this->peek() == '=') {
                    this->_right++;
                    return this->create(Type::GtEq);
                }

                return this->create(Type::Gt);
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

    Token* Scanner::create(Type type) {
        auto value = this->_src.substr(this->_left, this->_right - this->_left);

        // handle escaped characters
        if (type == Type::LString) {
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

        return new Token(
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

    Token* Scanner::on_comment() {
        while (this->peek() != '\n' && this->peek() != '\0') {
            this->_right++;
        }

        this->_ln++;
        this->_right++;
        return this->scan();
    }

    Token* Scanner::on_string() {
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
        auto token = this->create(Type::LString);
        this->_right++;
        return token;
    }

    Token* Scanner::on_number() {
        while (this->is_integer(this->peek())) {
            this->_right++;
        }

        if (this->peek() == '.') {
            this->_right++;

            while (this->is_integer(this->peek())) {
                this->_right++;
            }
        }

        return this->create(Type::LNumber);
    }

    Token* Scanner::on_identifier() {
        while (this->is_alpha(this->peek()) || this->is_integer(this->peek())) {
            this->_right++;
        }

        const string identifier = this->_src.substr(
            this->_left,
            this->_right - this->_left
        );

        if (keyword_to_type(identifier) != Type::Invalid) {
            return this->create(keyword_to_type(identifier));
        }

        return this->create(Type::Identifier);
    }
};

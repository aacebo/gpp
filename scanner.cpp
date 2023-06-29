#include "scanner.hpp"
#include "error.hpp"

namespace scanner {
    Scanner::Scanner(string src) : _src(src) {
        this->_left = 0;
        this->_right = 0;
        this->_ln = 0;
        this->scan();
    }

    Scanner::~Scanner() {
        for (auto token : this->_tokens) {
            delete token;
        }

        this->_tokens.clear();

        for (auto e : this->_errors) {
            delete e;
        }

        this->_errors.clear();
    }

    const vector<token::Token*> Scanner::get_tokens() {
        return this->_tokens;
    }

    const vector<error::Error*> Scanner::get_errors() {
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
                case '?':
                    this->push(token::Type::Optional);
                    break;
                case '-':
                    if (this->peek() == '=') {
                        this->_right++;
                        this->push(token::Type::MinusEq);
                    } else if (this->peek() == '>') {
                        this->_right++;
                        this->push(token::Type::ReturnType);
                    } else {
                        this->push(token::Type::Minus);
                    }

                    break;
                case '+':
                    if (this->peek() == '=') {
                        this->_right++;
                        this->push(token::Type::PlusEq);
                    } else {
                        this->push(token::Type::Plus);
                    }

                    break;
                case '*':
                    if (this->peek() == '=') {
                        this->_right++;
                        this->push(token::Type::StarEq);
                    } else {
                        this->push(token::Type::Star);
                    }

                    break;
                case '/':
                    if (this->peek() == '/') {
                        this->on_comment();
                    } else if (this->peek() == '=') {
                        this->_right++;
                        this->push(token::Type::SlashEq);
                    } else {
                        this->push(token::Type::Slash);
                    }

                    break;
                case '!':
                    if (this->peek() == '=') {
                        this->_right++;
                        this->push(token::Type::NotEq);
                    } else {
                        this->push(token::Type::Not);
                    }

                    break;
                case '=':
                    if (this->peek() == '=') {
                        this->_right++;
                        this->push(token::Type::EqEq);
                    } else {
                        this->push(token::Type::Eq);
                    }

                    break;
                case '<':
                    if (this->peek() == '=') {
                        this->_right++;
                        this->push(token::Type::LtEq);
                    } else {
                        this->push(token::Type::Lt);
                    }

                    break;
                case '>':
                    if (this->peek() == '=') {
                        this->_right++;
                        this->push(token::Type::GtEq);
                    } else {
                        this->push(token::Type::Gt);
                    }

                    break;
                case '"':
                    this->on_string();
                    break;
                default:
                    if (this->is_integer(c)) {
                        this->on_number();
                        break;
                    } else if (this->is_alpha(c)) {
                        this->on_identifier();
                        break;
                    }

                    this->_errors.push_back(new error::SyntaxError(
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

        this->_tokens.push_back(new token::Token(
            type,
            this->_ln,
            this->_left / this->_ln,
            (this->_left / this->_ln) + (this->_right - this->_left),
            value
        ));
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

    void Scanner::on_comment() {
        while (this->peek() != '\n' && this->peek() != '\0') {
            this->_right++;
        }

        this->_ln++;
        this->_right++;
    }

    void Scanner::on_string() {
        while ((this->peek() != '"' || this->is_escaped()) && this->peek() != '\0') {
            if (this->peek() == '\n') {
                this->_ln++;
            }

            this->_right++;
        }

        if (this->_right == this->_src.length()) {
            this->_errors.push_back(new error::SyntaxError(
                this->_ln,
                this->_left,
                this->_right,
                "unterminated string"
            ));

            return;
        }

        this->_left++;
        this->push(token::Type::LString);
        this->_right++;
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

        this->push(token::Type::LNumber);
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

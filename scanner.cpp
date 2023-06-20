#include <string>

#include "scanner.hpp"
#include "error.hpp"

using namespace std;

scanner::scanner(string src) {
    _src_ = src;
    _left_ = 0;
    _right_ = 0;
    _ln_ = 0;
    
    while (_right_ < src.length()) {
        _left_ = _right_;
        char c = src[_right_];
        _right_++;

        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                // ignore whitespace
                break;
            case '\n':
                _ln_++;
                break;
            case '(':
                push(token::type::_lparen_);
                break;
            case ')':
                push(token::type::_rparen_);
                break;
            case '{':
                push(token::type::_lbrace_);
                break;
            case '}':
                push(token::type::_rbrace_);
                break;
            case ',':
                push(token::type::_comma_);
                break;
            case '.':
                push(token::type::_dot_);
                break;
            case ';':
                push(token::type::_semi_colon_);
                break;
            case '/':
                if (peek() == '/') {
                    on_comment();
                } else if (peek() == '=') {
                    push(token::type::_slash_eq_);
                    _right_++;
                }

                break;
            case '"':
                on_string();
                break;
            default:
                if (is_integer(c)) {
                    on_number();
                    break;
                } else if (is_alpha(c)) {
                    on_identifier();
                    break;
                }

                _errors_.push_back(new error(_ln_, _right_, "unexpected character"));
        }
    }
}

scanner::~scanner() {
    for (auto t : this->_tokens_) {
        delete t;
    }

    for (auto e : this->_errors_) {
        delete e;
    }
}

void scanner::push(token::type type) {
    _tokens_.push_back(new token(
        type,
        _src_.substr(this->_left_, this->_right_ - this->_left_),
        this->_ln_
    ));
}

char scanner::peek() {
    if (this->_right_ >= this->_src_.length()) {
        return '\0';
    }

    return this->_src_[this->_right_];
}

bool scanner::is_integer(char c) {
    return c >= '0' && c <= '9';
}

bool scanner::is_alpha(char c) {
    return (
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_'
    );
}

void scanner::on_comment() {
    while (this->peek() != '\n' && this->peek() != '\0') {
        this->_right_++;
    }

    this->_ln_++;
    this->_right_++;
}

void scanner::on_string() {
    while (this->peek() != '"' && this->peek() != '\0') {
        if (this->peek() == '\n') {
            this->_ln_++;
        }

        this->_right_++;
    }

    if (this->_right_ == this->_src_.length()) {
        this->_errors_.push_back(new error(
            this->_ln_,
            this->_left_,
            "unterminated string"
        ));

        return;
    }

    this->_right_++;
    this->push(token::type::_string_);
}

void scanner::on_number() {
    while (this->is_integer(this->peek())) {
        this->_right_++;
    }

    if (this->peek() == '.') {
        this->_right_++;

        while (this->is_integer(this->peek())) {
            this->_right_++;
        }
    }

    this->push(token::type::_number_);
}

void scanner::on_identifier() {
    while (this->is_alpha(this->peek()) || this->is_integer(this->peek())) {
        this->_right_++;
    }

    const string identifier = this->_src_.substr(
        this->_left_,
        this->_right_ - this->_left_
    );

    if (token::keyword_to_type.count(identifier) == 1) {
        this->push(token::keyword_to_type[identifier]);
    } else {
        this->push(token::type::_identifier_);
    }
}
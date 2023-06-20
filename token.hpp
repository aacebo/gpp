#pragma once

#include <string>
#include <map>

using namespace std;

class token {
    public:
        enum type {
            _comma_,
            _dot_,
            _semi_colon_,
            _lparen_, _rparen_,
            _lbrace_, _rbrace_,

            // arithmetic
            _minus_, _minus_eq_,
            _plus_, _plus_eq_,
            _slash_, _slash_eq_,
            _star_, _star_eq_,

            // logical
            _not_, _not_eq_,
            _eq_, _eq_eq_,
            _gt_, _gt_eq_,
            _lt_, _lt_eq_,
            _and_, _or_,

            // literals
            _identifier_, _string_, _number_, _boolean_, _nil_,

            // keywords
            _if_, _else_, _for_, _let_,
            _func_, _return_,
            _class_, _super_, _this_,
            _package_, _import_,

            _eof_
        };

        inline static map<string, type> keyword_to_type {
            { "if", type::_if_ },
            { "else", type::_else_ },
            { "for", type::_for_ },
            { "let", type::_let_ },
            { "func", type::_func_ },
            { "return", type::_return_ },
            { "class", type::_class_ },
            { "super", type::_super_ },
            { "this", type::_this_ },
            { "package", type::_package_ },
            { "import", type::_import_ }
        };

        token(type, string, int);

        const type get_type() { return this->_type_; }
        const string get_lex() { return this->_lex_; }
        const int get_ln() { return this->_ln_; }

    private:
        type _type_;
        string _lex_;
        int _ln_;
};

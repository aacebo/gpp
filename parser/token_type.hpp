#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <any>
#include <string>

using namespace std;

namespace parser {
    enum class Type {
        Comma,
        Dot,
        SemiColon,
        LParen, RParen,
        LBrace, RBrace,
        Nilable,

        // arithmetic
        Minus, MinusEq,
        Plus, PlusEq,
        Slash, SlashEq,
        Star, StarEq,

        // logical
        Not, NotEq,
        Eq, EqEq,
        Gt, GtEq,
        Lt, LtEq,
        And, Or,

        // literals
        Identifier, LString, LNumber, Nil,

        // keywords
        If, Else, For, Let, Const,
        Fn, Return, ReturnType,
        Class, Super, Self, Pub,
        Use, True, False,
        String, Number, Bool,
        Print,

        Eof,
        Invalid
    };

    Type keyword_to_type(string);
    string type_to_string(Type);
};

#endif
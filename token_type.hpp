#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <any>
#include <string>

using namespace std;

namespace token {
    enum class Type {
        Comma,
        Dot,
        SemiColon,
        LParen, RParen,
        LBrace, RBrace,
        Optional,

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
        Fn, Return,
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
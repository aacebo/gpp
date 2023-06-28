#ifndef STATEMENT_TYPE_H
#define STATEMENT_TYPE_H

#include <string>

using namespace std;

namespace statement {
    enum class Type {
        Block,
        Function,
        Class,
        Expression,
        If,
        Print,
        Return,
        Let,
        For,
        Use
    };

    string type_to_string(Type);
};

#endif
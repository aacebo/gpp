#ifndef EXPRESSION_TYPE_H
#define EXPRESSION_TYPE_H

#include <string>

using namespace std;

namespace expression {
    enum class Type {
        Assign,
        Binary,
        Call,
        Get,
        Set,
        Grouping,
        Literal,
        Logical,
        Super,
        Self,
        Unary,
        Variable
    };

    string type_to_string(Type);
};

#endif
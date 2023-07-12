#ifndef VALUE_TYPE_H
#define VALUE_TYPE_H

#include <string>

using namespace std;

namespace value {
    enum class Type {
        Bool,
        Number,
        Nil,
        Object
    };

    string type_to_string(Type);
};

#endif
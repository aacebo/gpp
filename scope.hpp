#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <any>
#include <string>

#include "token.hpp"
#include "error.hpp"

using namespace std;

namespace scope {
    class Scope {
        Scope* _outer;
        map<string, any> _values;

        public:
            Scope() { }
            Scope(Scope* outer) : _outer(outer) { }

            any get(token::Token);
            bool has(token::Token);
            void define(token::Token, any);
            void assign(token::Token, any);
    };
};

#endif
#ifndef SCOPE_H
#define SCOPE_H

#include <unordered_map>
#include <any>
#include <string>

#include "token.hpp"
#include "error.hpp"
#include "var.hpp"

using namespace std;

namespace scope {
    class Scope {
        Scope* _outer;
        unordered_map<string, Var<any>*> _values;

        public:
            Scope() { }
            Scope(Scope* outer) : _outer(outer) { }
            ~Scope();

            Var<any>* get(token::Token);
            bool has(token::Token);
            void define(token::Token, any);
            void assign(token::Token, any);
    };
};

#endif
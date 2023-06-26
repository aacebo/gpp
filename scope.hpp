#ifndef SCOPE_H
#define SCOPE_H

#include <unordered_map>
#include <any>
#include <string>

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

            Var<any>* get(string);
            bool has(string);
            void define(string, any);
            void assign(string, any);
    };
};

#endif
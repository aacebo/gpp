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
        unordered_map<string, Var*> _values;

        public:
            Scope* parent;

            Scope() { }
            Scope(Scope* parent) : parent(parent) { }
            ~Scope();

            Var* get(string);
            bool has(string);
            void define(string, Var*);
            void assign(string, Var*);
    };
};

#endif
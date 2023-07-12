#ifndef SCOPE_H
#define SCOPE_H

#include <string>
#include <unordered_map>

#include "value.hpp"
#include "value_definition.hpp"

using namespace std;

namespace value {
    class Scope {
        unordered_map<string, Definition> definitions;
        unordered_map<string, Value> values;

        public:
            Scope* parent;

            Scope();
            Scope(Scope*);

            bool has_local(const string&);
            bool has(const string&);

            void define(const string&, Definition, Value);
            Definition& get_definition(const string&);

            void assign(const string&, Value);
            Value& resolve(const string&);
    };
};

#endif
#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <unordered_map>

#include "object.hpp"
#include "value.hpp"

namespace value {
    class Class : public Object {
        public:
            const string name;
            unordered_map<string, Type> members;

            Class(string, unordered_map<string, Type>);
    };

    class Instance : public Object {
        public:
            Class* def;
            unordered_map<string, Value*> values;

            Instance(Class*);
            ~Instance();

            bool has(string);
            Value* get(string);
            void set(string, Value*);
    };
};

#endif
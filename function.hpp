#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>

#include "object.hpp"
#include "chunk.hpp"

using namespace std;

namespace value {
    enum class FunctionType {
        Default,
        Closure
    };

    class Function : public Object {
        public:
            const FunctionType fn_type;
            const string name;
            const vector<value::Type> arg_types;
            const value::Type return_type;
            compiler::Chunk chunk;

            Function(string);
            Function(FunctionType);
            Function(FunctionType, string, vector<value::Type>, value::Type);
            Function(string, vector<value::Type>, value::Type);

            bool is_default();
            bool is_closure();
            string to_string();
    };

    class Closure : public Function {
        public:
            Closure();
            Closure(vector<value::Type>, value::Type);

            string to_string();
    };
};

#endif
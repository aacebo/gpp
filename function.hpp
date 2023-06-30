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
        Method,
        Closure,
        Native
    };

    class Function : public Object {
        public:
            const FunctionType fn_type;
            const string name;
            const int argc;
            parser::Chunk chunk;

            Function(FunctionType, string, int);
            Function(string, int);

            bool is_closure();
            bool is_method();
            bool is_native();
    };

    class Closure : public Function {
        public:
            Closure(string, int);
    };

    class Method : public Function {
        public:
            Method(string, int);
    };

    class Native : public Function {
        public:
            Native(string, int);
    };
};

#endif
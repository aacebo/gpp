#ifndef CALLABLE_H
#define CALLABLE_H

#include <any>

using namespace std;

namespace interpreter {
    class Interpreter;
};

namespace scope {
    class Callable {
        public:
            virtual any call(interpreter::Interpreter*, vector<any>) = 0;
    };
};

#endif
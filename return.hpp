#ifndef RETURN_H
#define RETURN_H

#include <any>
#include <string>
#include <exception>

using namespace std;

namespace scope {
    class Return : exception {
        public:
            any value;

            Return(any value) : value(value) { }

            const string what() {
                return "<return " + any_cast<string>(this->value) + ">";
            }
    };
};

#endif
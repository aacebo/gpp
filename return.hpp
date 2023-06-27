#ifndef RETURN_H
#define RETURN_H

#include <string>
#include <exception>

#include "var.hpp"

using namespace std;

namespace scope {
    class Return : exception {
        public:
            Var* value;

            Return(Var* value) : value(value) { }

            const string what() {
                return "<return " + this->value->to_string() + ">";
            }
    };
};

#endif
#ifndef STRING_H
#define STRING_H

#include <string>
#include <any>

#include "object.hpp"

using namespace std;

namespace value {
    class String : public Object {
        public:
            string value;

            String(const string& value);

            string to_string();
            bool equals(const String&);
            String* operator+(const String&);
            any call(const string&);
    };
};

#endif
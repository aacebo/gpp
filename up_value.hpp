#ifndef UP_VALUE_H
#define UP_VALUE_H

#include <string>

#include "object.hpp"
#include "value.hpp"

namespace value {
    class UpValue : public Object {
        public:
            Value* location;
            UpValue* next_up_value;

            UpValue();

            string to_string();
    };
};

#endif
#include "string.hpp"

namespace value {
    String::String(const string& value) : Object(ObjectType::String), value(value) {

    }

    string String::to_string() {
        return this->value;
    }

    bool String::equals(const String& other) {
        return this->value == other.value;
    }
    
    String* String::operator+(const String& other) {
        return new String(this->value + other.value);
    }

    any String::call(const string& method) {
        if (method == "len") {
            return static_cast<int>(this->value.length());
        }

        throw runtime_error("'" + method + "' is not a valid method for type 'string'");
    }
};

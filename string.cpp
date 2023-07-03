#include "string.hpp"

namespace value {
    String::String(const string& value) : Object(ObjectType::String), value(value) {

    }

    string String::to_string() {
        return this->value;
    }

    String* String::operator+(const String& other) {
        return new String(this->value + other.value);
    }

    bool String::operator==(const String& other) {
        return this->value == other.value;
    }
};
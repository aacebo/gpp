#include "up_value.hpp"

namespace value {
    UpValue::UpValue() : Object(ObjectType::UpValue) { }

    string UpValue::to_string() {
        return this->location->to_string();
    }
};

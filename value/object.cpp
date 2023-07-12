#include "object.hpp"

namespace value {
    Object::Object(ObjectType type) : type(type) { }

    bool Object::is_string() {
        return this->type == ObjectType::String;
    }

    bool Object::is_function() {
        return this->type == ObjectType::Function;
    }

    Object* Object::to_object() {
        return dynamic_cast<Object*>(this);
    }
};

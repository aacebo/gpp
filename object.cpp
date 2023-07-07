#include "object.hpp"

namespace value {
    Object::Object(ObjectType type) : type(type) { }

    bool Object::is_string() {
        return this->type == ObjectType::String;
    }

    bool Object::is_class() {
        return this->type == ObjectType::Class;
    }

    bool Object::is_instance() {
        return this->type == ObjectType::Instance;
    }
    
    bool Object::is_up_value() {
        return this->type == ObjectType::UpValue;
    }

    bool Object::is_function() {
        return this->type == ObjectType::Function;
    }

    Object* Object::to_object() {
        return dynamic_cast<Object*>(this);
    }
};

#ifndef OBJECT_H
#define OBJECT_H

namespace value {
    class Value;
    enum class Type;

    enum class ObjectType {
        Class,
        Instance,
        UpValue,
        Function
    };

    class Object {
        public:
            const ObjectType type;
            Object* next;

            Object(ObjectType);

            bool is_class();
            bool is_instance();
            bool is_up_value();
            bool is_function();
    };

    class UpValue : public Object {
        public:
            Value* location;
            UpValue* next_up_value;

            UpValue();
    };
};

#endif
#ifndef OBJECT_H
#define OBJECT_H

#include <string>

using namespace std;

namespace value {
    enum class ObjectType {
        String,
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
            virtual ~Object() = default;

            bool is_string();
            bool is_class();
            bool is_instance();
            bool is_up_value();
            bool is_function();

            virtual string to_string() = 0;
    };
};

#endif
#ifndef OBJECT_H
#define OBJECT_H

#include <string>

using namespace std;

namespace value {
    enum class ObjectType {
        String,
        Function
    };

    class Object {
        public:
            const ObjectType type;

            Object(ObjectType);
            virtual ~Object() = default;

            bool is_string();
            bool is_function();
            Object* to_object();

            virtual string to_string() = 0;
    };
};

#endif
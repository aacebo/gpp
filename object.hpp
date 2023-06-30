#ifndef OBJECT_H
#define OBJECT_H

namespace value {
    enum class Type {
        Method,
        Class,
        Closure,
        Function,
        Instance,
        Native,
        String,
        UpValue
    };

    class Object {
        public:
            const Type type;
            bool is_marked;
    };
};

#endif
#ifndef VALUE_DEFINITION_H
#define VALUE_DEFINITION_H

#include "value_type.hpp"

namespace value {
    class Definition {
        public:
            const Type type;
            const bool is_const = false;
            const bool is_nilable = false;

            Definition(Type, bool, bool);
            Definition(const Definition&);
            
            Definition& operator=(Definition&);
    };
};

#endif

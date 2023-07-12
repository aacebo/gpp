#include "value_definition.hpp"

namespace value {
    Definition::Definition(
        Type type,
        bool is_const,
        bool is_nilable
    ) : type(type),
        is_const(is_const),
        is_nilable(is_nilable) {
        
    }

    Definition::Definition(
        const Definition& other
    ) : Definition(
        other.type,
        other.is_const,
        other.is_nilable
    ) {
        
    }

    Definition& Definition::operator=(Definition& other) {
        return other;
    }
};

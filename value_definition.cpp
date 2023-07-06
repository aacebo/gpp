#include "value_definition.hpp"

namespace value {
    Definition::Definition(
        Type type,
        bool is_const,
        bool is_optional
    ) : type(type),
        is_const(is_const),
        is_optional(is_optional) {
        
    }

    Definition::Definition(
        const Definition& other
    ) : Definition(
        other.type,
        other.is_const,
        other.is_optional
    ) {
        
    }

    Definition& Definition::operator=(Definition& other) {
        return other;
    }
};

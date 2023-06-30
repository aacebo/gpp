#include "chunk.hpp"

namespace parser {
    Operation::Operation(uint8_t code, int ln) : code(code), ln(ln) {

    }

    void Chunk::push(uint8_t byte, int ln) {
        this->ops.push_back(Operation(byte, ln));
    }

    int Chunk::push(value::Value value) {
        this->values.push_back(value);
        return this->values.size() - 1;
    }
};

#include "chunk.hpp"

namespace compiler {
    uint8_t Chunk::at(size_t i) {
        return this->bytes[i];
    }

    value::Value Chunk::const_at(size_t i) {
        return this->consts[i];
    }

    int Chunk::size_bytes() {
        return this->bytes.size();
    }

    int Chunk::size_consts() {
        return this->consts.size();
    }

    void Chunk::push(uint8_t byte) {
        this->bytes.push_back(byte);
    }

    void Chunk::push(OpCode code) {
        this->bytes.push_back(static_cast<uint8_t>(code));
    }

    void Chunk::push_const(value::Value value) {
        this->consts.push_back(value);
        this->bytes.push_back(this->consts.size() - 1);
    }
};

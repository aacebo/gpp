#include "chunk.hpp"

namespace compiler {
    OpCode Chunk::at(size_t i) {
        return this->codes[i];
    }

    value::Value Chunk::const_at(size_t i) {
        return this->consts[i];
    }

    int Chunk::size() {
        return this->codes.size();
    }

    void Chunk::push(uint8_t byte) {
        this->locations.push_back(byte);
    }

    void Chunk::push(OpCode code) {
        this->codes.push_back(code);
    }

    void Chunk::push_const(value::Value value) {
        this->consts.push_back(value);
        this->locations.push_back(this->consts.size() - 1);
    }
};

#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

#include "../value/value.hpp"

#include "op_code.hpp"

using namespace std;

namespace compiler {
    class Chunk {
        vector<uint8_t> bytes;
        vector<value::Value> consts;

        public:
            Chunk() = default;

            uint8_t at(size_t);
            value::Value const_at(size_t);
            int size_bytes();
            int size_consts();
            void push(uint8_t);
            void push(OpCode);
            int push_const(value::Value);
            void set(size_t, uint8_t);
    };
};

#endif
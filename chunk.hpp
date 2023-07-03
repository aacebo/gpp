#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

#include "value.hpp"
#include "op_code.hpp"

using namespace std;

namespace compiler {
    class Chunk {
        vector<compiler::OpCode> codes;
        vector<uint8_t> locations;
        vector<value::Value> consts;

        public:
            Chunk() = default;

            OpCode at(size_t);
            value::Value const_at(size_t);
            int size();
            void push(uint8_t);
            void push(OpCode);
            void push_const(value::Value);
    };
};

#endif
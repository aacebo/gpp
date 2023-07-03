#ifndef FRAME_H
#define FRAME_H

#include "function.hpp"
#include "op_code.hpp"

namespace vm {
    class Frame {
        uint8_t ip; // instruction pointer
        value::Closure* closure;

        public:
            Frame(value::Closure*);
            ~Frame();

            bool is_empty();
            compiler::OpCode next_code();
            uint8_t next_byte();
            value::Value next_const();
    };
};

#endif
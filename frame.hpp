#ifndef FRAME_H
#define FRAME_H

#include "function.hpp"
#include "op_code.hpp"

namespace vm {
    class Frame {
        uint8_t ip; // instruction pointer
        uint8_t lp; // const location pointer
        value::Closure* closure;

        public:
            Frame(value::Closure*);
            ~Frame();

            bool is_empty();
            compiler::OpCode next();
            value::Value next_const();
    };
};

#endif
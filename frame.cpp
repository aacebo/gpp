#include "frame.hpp"

namespace vm {
    Frame::Frame(value::Closure* closure) : closure(closure) {
        this->ip = -1;
        this->lp = -1;
    }

    Frame::~Frame() {
        delete this->closure;
    }

    bool Frame::is_empty() {
        return this->ip == this->closure->chunk.size() - 1;
    }

    compiler::OpCode Frame::next() {
        this->ip++;
        return this->closure->chunk.at(this->ip);
    }

    value::Value Frame::next_const() {
        this->lp++;
        return this->closure->chunk.const_at(this->lp);
    }
};

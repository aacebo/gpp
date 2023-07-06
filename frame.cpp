#include "frame.hpp"

namespace vm {
    Frame::Frame(value::Function* closure) : closure(closure) {
        this->ip = -1;
    }

    Frame::~Frame() {
        delete this->closure;
    }

    bool Frame::is_empty() {
        return this->ip == this->closure->chunk.size_bytes() - 1;
    }

    compiler::OpCode Frame::next_code() {
        this->ip++;
        return static_cast<compiler::OpCode>(this->closure->chunk.at(this->ip));
    }

    uint8_t Frame::next_byte() {
        this->ip++;
        return this->closure->chunk.at(this->ip);
    }

    value::Value Frame::next_const() {
        return this->closure->chunk.const_at(this->next_byte());
    }

    uint16_t Frame::next_short() {
        auto a = this->next_byte();
        auto b = this->next_byte();
        return (a << 8) | b;
    }

    void Frame::jump_inc(int offset) {
        this->ip += offset;
    }

    void Frame::jump_dec(int offset) {
        this->ip -= offset;
    }
};

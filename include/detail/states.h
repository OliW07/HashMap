#pragma once

#include <cstdint>

enum class State { null = 0, empty = 1 << 0, occupied = 1 << 1, deleted = 1 << 2};

class StateSet {
    private:
        uint8_t bits_;



    public:
        StateSet() : bits_(0) {}

        explicit StateSet(State s1 = State::null, State s2 = State::null, State s3=State::null){
            bits_ = static_cast<uint8_t>(s1) | static_cast<uint8_t>(s2) | static_cast<uint8_t>(s3);
        }


};

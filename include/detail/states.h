#pragma once

#include <string>
#include <cstdint>
#include <initializer_list>

enum class State { empty = 1 << 0, occupied = 1 << 1, deleted = 1 << 2};

inline std::string to_string(State state) {
    switch(state){
        case State::empty:
            return "State::empty";
        case State::occupied:
            return "State::occupied";
        case State::deleted:
            return "State::deleted";
        default:
            return "unknown state";

    }
}

class StateSet {
    private:
        uint8_t bits_ = 0;

    public:

        StateSet() = default;
        StateSet(std::initializer_list<State> states){
            for(State state : states){
                bits_ |= static_cast<uint8_t>(state);
            }
        }

        bool contains(State state) const {
            return (bits_ & static_cast<uint8_t>(state)) != 0;
        }

        std::string format() const {
            std::string res;
            for(State state : {State::empty, State::deleted, State::occupied}){
                if(contains(state)){
                    if(!res.empty())
                        res += " | ";
                    res += to_string(state);
                }
            }

            return res;
        }


};

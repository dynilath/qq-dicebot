#pragma once

#include "./common.h"

namespace dicebot::manual{
    using vec_mdice = std::vector<std::pair<int,int>>;
    using pair_mdice = std::pair<int,int>;
    class manual_dice: vec_mdice{
    public:
        manual_dice();
        manual_dice(const std::string & source);
        void roll(const std::string & source);
        void kill(const std::string & source);
        void add(const std::string & source);
        void killall();
        std::string encode() const;
        std::string str();

        roll::roll_status status;
        int i_sum_result;
        
        void decode(std::string & source);
        explicit operator bool() const noexcept;
    };
}
#pragma once

#include <string>
#include <vector>
#include "../constants.h"

namespace dicebot::manual {
    using vec_mdice = std::vector<std::pair<int, int>>;
    using pair_mdice = std::pair<int, int>;
    class manual_dice {
    public:
        vec_mdice mdice;
        manual_dice();
        void roll(size_t target) noexcept;
        void kill(size_t target) noexcept;
        void add(const std::vector<int>&);
        void killall();
        std::string encode() const;
        explicit operator std::string() const noexcept;

        int i_sum_result;

        void decode(const std::string& source);
    };
} // namespace dicebot::manual
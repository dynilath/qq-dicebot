#pragma once
#include <random>

namespace dicebot::random {
    void initialize();
    int rand_int(int min, int max);
    int rand_int(std::uniform_int_distribution<int>&);
    std::uniform_int_distribution<int> create_distribution(int min, int max);
} // namespace dicebot::random

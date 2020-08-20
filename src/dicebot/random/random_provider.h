#pragma once
#include <random>

namespace dicebot::random {
    using distribution_t = ::std::uniform_int_distribution<int>;
    void initialize();
    int rand_int(int min, int max);
    int rand_int(distribution_t const&);
    distribution_t create_distribution(int min, int max);
} // namespace dicebot::random

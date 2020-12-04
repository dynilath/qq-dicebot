#include "./random_provider.h"

#include <chrono>
#include <random>

#include "../../utils/xoroshiro128starstar.hpp"

using namespace dicebot;
using namespace dicebot::random;

static bool is_random_device_unavail = false;

//static std::random_device rd_generator;
//static std::mt19937 mt_generator;
static xoroshiro128starstar xororshiro;

void random::initialize() {
    ::std::random_device rd;
    //::std::seed_seq seed {rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    //mt_generator.seed(seed);
    xororshiro.seed(rd(),rd());
}

int random::rand_int(int min, int max) {
    if (min > max) return max;
    std::uniform_int_distribution<int> dice_distribution(min, max);
    return dice_distribution(xororshiro);
}

int random::rand_int(distribution_t const& distribution) {
    return distribution(xororshiro);
}

distribution_t random::create_distribution(int min, int max) {
    if (min > max) min = max;
    return distribution_t(min, max);
}
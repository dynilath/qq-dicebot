#include "./random_provider.h"

#include <chrono>
#include <random>

using namespace dicebot;
using namespace dicebot::random;

static bool is_random_device_unavail = false;

static std::random_device rd_generator;
static std::mt19937 mt_generator;

void random::initialize() {
    if (rd_generator.entropy() > 0.0) {
        is_random_device_unavail = false;
    } else {
        mt_generator.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
        is_random_device_unavail = true;
    }
}

int random::rand_int(int min, int max) {
    if (min > max) min = max;
    std::uniform_int_distribution<int> dice_distribution(min, max);
    if (is_random_device_unavail) {
        return dice_distribution(mt_generator);
    } else {
        return dice_distribution(rd_generator);
    }
}

int random::rand_int(std::uniform_int_distribution<int>& distribution) {
    if (is_random_device_unavail) {
        return distribution(mt_generator);
    } else {
        return distribution(rd_generator);
    }
}

std::uniform_int_distribution<int> random::create_distribution(int min, int max) {
    if (min > max) min = max;
    return std::uniform_int_distribution<int>(min, max);
}
#include "./common.h"

namespace dicebot::logger{
    std::function<void(std::string,std::string)> log;
}
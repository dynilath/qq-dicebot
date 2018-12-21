#pragma once

#include <map>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <regex>
#include <sstream>
#include <memory>

#include <functional>

namespace dicebot{

    namespace roll{
        enum class roll_status{
            FINISHED = 0,
            GENERAL_ERR = 1,
            DICE_NOT_AVAILABLE = 2,
            UNINITIALIZED = -1,
            TOO_MANY_DICE = 3
        };

        enum class roll_mode{
            DND_DK = 1,
            COC_PB = 2,
            WOD_WA = 3
        };
    }

    namespace logger{
        extern std::function<void(std::string,std::string)> log;
    }
    
    #define MAX_DICE_UNIT_COUNT 10
    #define MAX_DICE_NUM 50
    #define MAX_DICE_FACE 1000
}
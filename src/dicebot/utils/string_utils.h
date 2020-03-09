#pragma once
#include <list>
#include <string>

#include "./string_view.h"

namespace dicebot::utils {
    std::list<utils::string_view> split_line(std::string const&);
    bool trim(utils::string_view &);
    bool jump_to_front_of_point(utils::string_view &);

} // namespace dicebot::utils

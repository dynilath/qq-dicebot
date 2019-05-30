#pragma once

#include <list>
#include <sstream>

#include "../event_info.h"

namespace dicebot::entry {

    using dice_request = std::string *(std::string const &, event_info const &, std::string &);
    class entry_base {
    public:
        bool is_stand_alone = false;
        std::string identifier_regex;
        std::list<std::string> identifier_list;
        std::string help_message;
        virtual bool resolve_request(std::string const &message, event_info &event, std::string &response) noexcept {
            return false;
        }
        virtual ~entry_base() = default;
    };

} // namespace dicebot::entry

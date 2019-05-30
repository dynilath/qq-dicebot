#pragma once
#include <list>
#include <string>

namespace dicebot::utils {

    struct string_part : public std::pair<std::string::const_iterator, std::string::const_iterator> {
        using iter_t = std::string::const_iterator;
        string_part(iter_t first, iter_t second) : std::pair<iter_t, iter_t>(first, second){};
        operator std::string() { return std::string(this->first, this->second); }
    };
    void split_line_part(std::string const&, std::list<string_part>& source_splits);
    bool trim_part(std::string const&, string_part&);
    bool jump_to_point_part(std::string const&, string_part&);
} // namespace dicebot::utils

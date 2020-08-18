#include "./string_utils.h"

#include <algorithm>
#include <array>
#include <functional>
#include <locale>

using namespace dicebot;

constexpr std::array<char, 2> new_line_array = {'\r', '\n'};
std::default_searcher new_line_searcher(new_line_array.begin(),
                                        new_line_array.end());

std::list<utils::string_view> utils::split_line(std::string const& source) {
    std::list<utils::string_view> ret;
    auto start = source.begin();
    auto target = start;

    while (true) {
        auto target = std::search(start, source.end(), new_line_searcher);
        if (std::distance(start, target) > 0) ret.emplace_back(start, target);
        if (target == source.end()) break;
        start = target + new_line_array.size();
    }

    return ret;
}

std::list<utils::string_view> utils::split_line_corn(std::string const& source) {
    std::list<utils::string_view> ret;
    auto start = source.begin();

    while (true) {
        auto target = ::std::find(start, source.end(), '\r');
        if (std::distance(start, target) > 0) ret.emplace_back(start, target);
        if (target == source.end()) break;
        start = target + 1;
    }

    return ret;
}

bool utils::trim(utils::string_view& part) {
    if (part.empty()) return false;
    auto left = part.begin();
    while (std::isblank(*left, std::locale("")) && left != part.end()) left++;
    if (left == part.end()) return false;
    auto right = part.rbegin();
    while (std::isblank(*right, std::locale("")) && right != part.rend())
        right++;
    if (right == part.rend()) return false;
    part = {left, right.base()};
    return true;
}

bool utils::jump_to_front_of_point(utils::string_view& part) {
    auto left = part.begin();
    while (std::isblank(*left, std::locale("")) && left != part.end()) left++;
    if (std::distance(left, part.end()) < 2) return false;
    part = {left + 1, part.end()};
    return *left == '.';
}
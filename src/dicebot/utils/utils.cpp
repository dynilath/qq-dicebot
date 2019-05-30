#include "./utils.h"
#include <algorithm>
#include <locale>
#include <vector>

using namespace dicebot;

void utils::remove_blank(std::string& source) {
    std::vector<char> container;
    container.reserve(source.size() + 1);
    for (auto& item : source) {
        if (std::isblank(item, std::locale(""))) continue;
        container.push_back(item);
    }
    container.push_back('\0');
    source.assign(container.data());
}

void utils::lower_case(std::string& target) { std::transform(target.cbegin(), target.cend(), target.begin(), tolower); }

std::string utils::lower_case_copy(const std::string& target) {
    std::vector<char> container;
    container.reserve(target.size() + 1);
    std::transform(target.cbegin(), target.cend(), std::back_inserter(container), tolower);
    container.push_back('\0');
    return std::string(container.data());
}
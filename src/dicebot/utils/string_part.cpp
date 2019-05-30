#include "./string_part.h"
#include <algorithm>
#include <locale>

using namespace dicebot;

constexpr char new_line[] = "\r\n";
constexpr size_t new_line_len = 2;
void utils::split_line_part(std::string const& source, std::list<string_part>& source_splits) {
    size_t o_pos = 0;
    size_t f_pos;
    while (o_pos < source.size()) {
        f_pos = source.find(new_line, o_pos);
        if (f_pos == std::string::npos) {
            source_splits.emplace_back(source.begin() + o_pos, source.end());
            return;
        }
        if (f_pos > o_pos) source_splits.emplace_back(source.begin() + o_pos, source.begin() + f_pos);
        o_pos = f_pos + new_line_len;
    }
}

bool utils::trim_part(std::string const& source, utils::string_part& part) {
    while (std::isblank(*part.first, std::locale("")) && part.first != source.end()) part.first++;
    if (part.first == source.end()) return false;
    while (std::isblank(*(part.second - 1), std::locale(""))) part.second--;
    return true;
}

bool utils::jump_to_point_part(std::string const& source, utils::string_part& part) {
    while (std::isblank(*part.first, std::locale("")) && part.first != source.end()) part.first++;
    if (part.first == source.end())
        return false;
    else if (*part.first == '.') {
        part.first++;
        return true;
    } else
        return false;
}
#include "./manual_dice.h"

#include <iostream>
#include <regex>
#include <sstream>

#include "../../cqcppsdk/src/utils/base64.h"
#include "../dice_excepts.h"
#include "../dice_roller.h"
#include "../random/random_provider.h"

using namespace dicebot;
using namespace dicebot::manual;

static auto check_limits = [](int num, int face) {
    if (num > MAX_DICE_NUM) throw dice_exceed();
    if (face > MAX_DICE_FACE) throw face_exceed();
    if (face < 1 || num < 1) throw invalid_dice();
};

std::regex regex_filter_manual_dice_part("^(?:\\+)?(\\d+)?[dD](\\d+)");

manual_dice::manual_dice() { i_sum_result = 0; }

void manual_dice::roll(size_t target) noexcept {
    if (target > this->mdice.size() || target < 1) {
        return;
    }

    auto& item = this->mdice.at(target - 1);
    this->i_sum_result -= item.second;
    item.second = random::rand_int(1, item.first);
    this->i_sum_result += item.second;
}

void manual_dice::kill(size_t target) noexcept {
    if (target > this->mdice.size() || target < 1) {
        return;
    }
    auto iter = this->mdice.begin() + target - 1;
    i_sum_result -= (*iter).second;
    this->mdice.erase(iter);
}

void manual_dice::add(const std::vector<int>& source) {
    std::regex regex_manual_part("^(?:\\+)?(\\d+)?[dD](\\d+)");

    size_t fin_size = source.size() + this->mdice.size();
    this->mdice.reserve(source.size());
    for (auto& item : source) {
        check_limits(fin_size, item);
        this->mdice.emplace_back(item, random::rand_int(1, item));
    }
    this->i_sum_result = 0;
    for (auto& item : this->mdice) {
        this->i_sum_result += item.second;
    }
}

void manual_dice::killall() {
    this->mdice.clear();
    this->i_sum_result = 0;
}

std::string manual_dice::encode() const {
    std::ostringstream strs;
    strs << this->mdice.size() << " ";
    for (auto& item : this->mdice) {
        strs << item.first << " " << item.second << " ";
    }
    return cq::utils::base64_encode((const unsigned char*)(strs.str().c_str()), strs.str().size());
}

void manual_dice::decode(const std::string& source) {
    this->mdice.clear();
    std::string source_copy = cq::utils::base64_decode(source);
    std::istringstream iss(source_copy);

    this->i_sum_result = 0;
    int len = 0;
    iss >> len;
    for (int i_iter = 0; i_iter < len; i_iter++) {
        int first, second;
        iss >> first >> second;
        this->i_sum_result += second;
        this->mdice.push_back(pair_mdice(first, second));
    }
}

manual_dice::operator std::string() const noexcept {
    std::ostringstream ostrs_result;
    int i_sum_result = 0;
    bool is_first = true;
    for (auto& item : this->mdice) {
        if (is_first)
            is_first = false;
        else
            ostrs_result << " + ";
        ostrs_result << item.second << "(" << item.first << ")";
        i_sum_result += item.second;
    }

    if (this->mdice.empty())
        ostrs_result << u8"没有骰子了";
    else
        ostrs_result << " = " << i_sum_result;
    return ostrs_result.str();
}
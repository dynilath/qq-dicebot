#include "./entry_specialized_dice.h"

#include <functional>

#include "../dice_roller.h"
#include "../utils/utils.h"
#include "./wrappers.h"

using namespace dicebot;
using namespace dicebot::entry;

#pragma region wod

static const std::regex wod_full_dice("^(\\d+)(?:d(\\d+))?(?:b(\\d+))? *", std::regex_constants::icase);

static const std::regex wod_filter_command("^(n|o) *", std::regex_constants::icase);

static bool roll_owod(std::string const& message, const event_info& event, std::string& response) {
    std::smatch command_match;
    std::regex_search(message, command_match, wod_full_dice);
    if (!command_match.empty()) {
        try {
            int dice = stoi(command_match[1]);

            std::ostringstream strs;
            strs << "oWoD";

            int difficulty = 6;
            if (command_match[2].matched) {
                difficulty = stoi(command_match[2]);
                strs << " D" << difficulty;
            }

            int bonus = 11;
            if (command_match[3].matched) {
                bonus = stoi(command_match[3]);
                if (bonus < 6) bonus = 6;
                strs << " B" << bonus;
            }

            roll::dice_roll dr;
            roll::roll_wod(dr, dice, difficulty, bonus, true);
            output_constructor oc(event.nickname, command_match.suffix());
            oc.append_roll(strs.str(), dr.detail(), dr.summary);
            response = oc;
            return true;
        } catch (std::invalid_argument&) {
            return false;
        } catch (std::out_of_range&) {
            return false;
        }
    }
    return false;
};

static bool roll_nwod(std::string const& message, const event_info& event, std::string& response) {
    std::smatch command_match;
    std::regex_search(message, command_match, wod_full_dice);
    if (!command_match.empty()) {
        try {
            int dice = stoi(command_match[1]);

            std::ostringstream strs;
            strs << "nWoD";

            int difficulty = 8;
            if (command_match[2].matched) {
                difficulty = stoi(command_match[2]);
                strs << " D" << difficulty;
            }

            int bonus = 10;
            if (command_match[3].matched) {
                bonus = stoi(command_match[3]);
                if (bonus < 6) bonus = 6;
                strs << " B" << bonus;
            }

            roll::dice_roll dr;
            roll::roll_wod(dr, dice, difficulty, bonus, false);
            output_constructor oc(event.nickname, command_match.suffix());
            oc.append_roll(strs.str(), dr.detail(), dr.summary);
            response = oc;
            return true;
        } catch (std::invalid_argument&) {
            return false;
        } catch (std::out_of_range&) {
            return false;
        }
    }
    return false;
}

using wod_call = std::function<bool(std::string const&, const event_info&, std::string&)>;
using wod_map_t = std::map<std::string, wod_call>;

static const wod_map_t wod_map = {{"n", roll_nwod}, {"o", roll_owod}};

entry_wod_dice::entry_wod_dice() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "w(?:od)?";
    this->identifier_list = {"wod", "w"};

    this->help_message =
        u8"WoD定制骰子(.wod或.w)\n"
        u8"指令.wo4：使用owod规则进行骰池为4的检定\n"
        u8"指令.wn4：使用nwod规则进行骰池为4的检定\n"
        u8"指令.wo4d8：指定难度为8\n"
        u8"指令.wn4b8：指定在8或以上获得奖励骰";
}

static bool wod_request_with_except(std::string const& message, event_info& event, std::string& response) {
    std::smatch command_match;
    std::regex_search(message, command_match, wod_filter_command);
    if (command_match.empty()) return false;

    std::string str_match = command_match[1];
    utils::lower_case(str_match);

    auto iter = wod_map.find(str_match);
    if (iter != wod_map.end()) {
        return iter->second(command_match.suffix().str(), event, response);
    }

    return false;
}
bool entry_wod_dice::resolve_request(std::string const& message, event_info& event, std::string& response) noexcept {
    return shoot_exceptions(wod_request_with_except, message, event, response);
}
#pragma endregion

#pragma region coc

entry_coc_dice::entry_coc_dice() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "c(?:oc)?";
    this->identifier_list = {"coc", "c"};

    this->help_message =
        u8"CoC定制骰子(.coc或.c)\n"
        u8"指令.coc：coc定制骰子\n"
        u8"指令.c：上述命令的简写形式\n"
        u8"指令.coc p1：惩罚骰1（penalty 1）\n"
        u8"指令.coc b1：奖励骰1（bonus 1）";
}

static const std::regex coc_full_dice("^([pb]\\d+ *)* *", std::regex_constants::icase);
static const std::regex coc_single_dice("^([pb])(\\d+) *", std::regex_constants::icase);

static bool coc_request_with_except(std::string const& message, event_info& event, std::string& response) {
    std::string::const_iterator work_point = message.begin();
    std::string::const_iterator end_point = message.end();

    std::smatch roll_match;
    std::ostringstream roll_source;
    roll_source << "CoC";
    int i_pb = 0;
    bool is_pb_exist = false;
    try {
        std::regex_search(work_point, end_point, roll_match, coc_single_dice);
        while (!roll_match.empty()) {
            if (!is_pb_exist) {
                roll_source << " ";
                is_pb_exist = true;
            }
            if (*(roll_match[1].first) == 'p') {
                roll_source << 'p' << roll_match[2];
                i_pb -= stoi(roll_match[2]);
            } else {
                roll_source << 'b' << roll_match[2];
                i_pb += stoi(roll_match[2]);
            }
            work_point += roll_match[0].length();
            std::regex_search(work_point, end_point, roll_match, coc_single_dice);
        }
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }

    roll::dice_roll dr;
    roll::roll_coc(dr, i_pb);
    output_constructor oc(event.nickname);
    if (is_pb_exist)
        oc.append_message(roll_match.suffix().str());
    else
        oc.append_message(message);
    oc.append_roll(roll_source.str(), dr.detail_coc(), dr.summary);
    response = oc;

    return true;
}

bool entry_coc_dice::resolve_request(std::string const& message, event_info& event, std::string& response) noexcept {
    return shoot_exceptions(coc_request_with_except, message, event, response);
}
#pragma endregion

#pragma region fate
entry_fate_dice::entry_fate_dice() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "f(?:ate)?";
    this->identifier_list = {"fate", "f"};

    this->help_message =
        u8"FATE定制骰子(.fate或.f)\n"
        u8"指令.f：常规FATE骰\n"
        u8"指令.f+4：指定+4修正";
}

static const std::regex fate_full_dice("^([\\+\\-]\\d+) *");

bool entry_fate_dice::resolve_request(std::string const& message, event_info& event, std::string& response) noexcept {
    std::string::const_iterator work_point = message.begin();
    std::string::const_iterator end_point = message.end();

    std::smatch roll_match;
    std::regex_search(work_point, end_point, roll_match, fate_full_dice);

    int i_modifier = 0;
    bool is_modifier_exist = false;
    std::ostringstream roll_source;
    roll_source << "FATE";
    try {
        if (!roll_match.empty()) {
            roll_source << " " << roll_match[1];
            i_modifier = stoi(roll_match[1]);
            is_modifier_exist = true;
        }
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }

    roll::dice_roll dr;
    roll::roll_fate(dr, i_modifier);

    output_constructor oc(event.nickname);
    if (is_modifier_exist)
        oc.append_message(roll_match.suffix().str());
    else
        oc.append_message(message);
    oc.append_roll(roll_source.str(), dr.detail_fate(), dr.summary);
    response = oc;
    return true;
}
#pragma endregion
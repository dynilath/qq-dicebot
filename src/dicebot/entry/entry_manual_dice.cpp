#include "./entry_manual_dice.h"

#include <regex>

#include "../data/manual_dice_control.h"
#include "../dice_excepts.h"
#include "../dice_roller.h"
#include "../entity/manual_dice.h"
#include "../utils/utils.h"
#include "./output_constructor.h"

using namespace dicebot;
using namespace dicebot::entry;

using md_ctrl = manual::manual_dice_control;

static const std::regex filter_integer("^(\\d+) *");
static const std::regex filter_manual_dice("^ *((\\+)?\\d*d\\d+)(\\+\\d*d\\d+)* *", std::regex_constants::icase);

static const std::regex single_dice("^(?:\\+)?(\\d*)d(\\d+) *", std::regex_constants::icase);

static auto cur_state = [](output_constructor& oc, auto&& state) { oc << u8" | 当前状态: " << std::string(state); };

static bool manualdice_dissemble(std::string const& message, std::vector<int>& dice_cont, std::string& str_cmd,
                                 std::string& roll_msg) {
    std::smatch roll_match;
    auto work_point = message.begin();
    auto end_point = message.end();
    bool is_cmd_exist = false;

    std::regex_search(work_point, end_point, roll_match, single_dice);
    while (!roll_match.empty()) {
        size_t count = 1;
        if (roll_match[1].length()) count = stoi(roll_match[1]);
        int dface = stoi(roll_match[2]);
        while (count--) {
            dice_cont.push_back(dface);
        }
        if (dice_cont.size() > MAX_DICE_NUM) throw dice_exceed();

        str_cmd.append(roll_match[1]).append("d").append(roll_match[2]);
        if (!is_cmd_exist)
            is_cmd_exist = true;
        else
            str_cmd.append("+");

        work_point += roll_match[0].length();
        std::regex_search(work_point, end_point, roll_match, filter_manual_dice);
    }
    if (is_cmd_exist) {
        roll_msg = roll_match.suffix();
    }
    return is_cmd_exist;
}

static bool manualdice_add(std::string const& message, const event_info& event, std::string& response) {
    std::vector<int> dice_cont;
    bool is_cmd_exist = false;
    std::string str_cmd;
    std::string roll_msg;
    try {
        is_cmd_exist = manualdice_dissemble(message, dice_cont, str_cmd, roll_msg);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }

    if (is_cmd_exist) {
        auto md_target = md_ctrl::get_instance()->find_manual_dice(event);
        md_target->second.add(dice_cont);
        md_ctrl::get_instance()->sync_database(md_target);

        output_constructor oc(event.nickname, roll_msg);
        oc << u8"在桌上增加了这些骰子:" << str_cmd;
        cur_state(oc, md_target->second);
        response = oc;
        return true;
    }
    return false;
};

static auto manualdice_killall = [](std::string const& message, const event_info& event, std::string& response) noexcept -> bool {
    auto md_target = md_ctrl::get_instance()->find_manual_dice(event);
    md_target->second.killall();
    md_ctrl::get_instance()->sync_database(md_target);

    output_constructor oc(event.nickname, message);
    oc << u8"杀掉了所有的骰子";
    cur_state(oc, md_target->second);
    response = oc;
    return true;
};

static bool manualdice_kill(std::string const& message, const event_info& event, std::string& response) {
    std::smatch roll_match;
    std::regex_search(message, roll_match, filter_integer);
    if (!roll_match.empty()) {
        auto md_target = md_ctrl::get_instance()->find_manual_dice(event);

        try {
            md_target->second.kill(stoi(roll_match[1]));
            md_ctrl::get_instance()->sync_database(md_target);
        } catch (const std::invalid_argument&) {
            return false;
        } catch (const std::out_of_range&) {
            return false;
        }

        output_constructor oc(event.nickname, roll_match.suffix());
        oc << u8"杀死桌上的第" << roll_match[1] << u8"个骰子";
        cur_state(oc, md_target->second);
        response = oc;

        return true;
    }
    return false;
};

static bool manualdice_roll(std::string const& message, const event_info& event, std::string& response) {
    std::smatch roll_match;
    std::regex_search(message, roll_match, filter_integer);
    if (!roll_match.empty()) {
        auto md_target = md_ctrl::get_instance()->find_manual_dice(event);

        try {
            md_target->second.roll(stoi(roll_match[1]));
            md_ctrl::get_instance()->sync_database(md_target);
        } catch (const std::invalid_argument&) {
            return false;
        } catch (const std::out_of_range&) {
            return false;
        }

        output_constructor oc(event.nickname, roll_match.suffix());
        oc << u8"重骰桌上的第" << roll_match[1] << u8"个骰子";
        cur_state(oc, md_target->second);
        response = oc;
        return true;
    }
    return false;
}

static bool manualdice_create(std::string const& message, const event_info& event, std::string& response) {
    std::vector<int> dice_cont;
    bool is_cmd_exist = false;
    std::string str_cmd;
    std::string roll_msg;
    try {
        is_cmd_exist = manualdice_dissemble(message, dice_cont, str_cmd, roll_msg);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
    if (!is_cmd_exist) return false;

    auto md_target = md_ctrl::get_instance()->find_manual_dice(event);
    md_target->second.killall();
    md_target->second.add(dice_cont);
    md_ctrl::get_instance()->sync_database(md_target);

    output_constructor oc(event.nickname, roll_msg);
    oc << u8"在桌上放了这些骰子: " << str_cmd;
    cur_state(oc, md_target->second);
    response = oc;
    return true;
}

using func_type = std::function<bool(std::string const& message, const event_info&, std::string&)>;
using func_map_t = std::unordered_map<std::string, func_type>;

static const func_map_t func_map = {{"ka", manualdice_killall},
                                    {"a", manualdice_add},
                                    {"k", manualdice_kill},
                                    {"r", manualdice_roll},
                                    {"killall", manualdice_killall},
                                    {"add", manualdice_add},
                                    {"kill", manualdice_kill},
                                    {"roll", manualdice_roll}};

std::regex filter_command("^(ka|a|k|r|killall|add|kill|roll) *", std::regex_constants::icase);

entry_manual_dice::entry_manual_dice() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "m(?:annual)?";
    this->identifier_list = {"mannual", "m"};
    this->help_message =
        u8"手动骰子(.manual或者.m)\n"
        u8"指令.m [dice]：产生手动骰子，后接骰子，这里不支持算式\n"
        u8"指令.mr [number]：骰指定骰子，后接骰子序号\n"
        u8"指令.mk [number]：消灭指定骰子，后接骰子序号\n"
        u8"指令.mka：清空所有手动骰子";
}

bool entry_manual_dice::resolve_request(std::string const& message, event_info& event, std::string& response) noexcept {
    std::string str_nickname = event.nickname;

    std::smatch match_command;
    std::regex_search(message, match_command, filter_command);
    if (match_command.empty()) return manualdice_create(message, event, response);

    std::string str_match = match_command[1];
    utils::lower_case(str_match);

    auto iter = func_map.find(str_match);
    if (iter != func_map.end()) {
        return iter->second(match_command.suffix().str(), event, response);
    }
    return false;
}
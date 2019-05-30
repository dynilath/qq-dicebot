#include "./entry_roll_dice.h"

#include <regex>

#include "../data/profile_manager.h"
#include "../utils/utils.h"
#include "./wrappers.h"

// regular dice, with detailed info

using namespace dicebot;
using namespace dicebot::entry;

entry_roll_dice::entry_roll_dice() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "r(?:oll)?";
    this->identifier_list = {"roll", "r"};
    this->help_message =
        u8"骰子指令(.roll或.r)\n"
        u8"指令.r 1d20：骰一个1d20\n"
        u8"指令.r 1d20+2d6+4：骰1d20+2d6+4，并输出其计算结果\n"
        u8"指令.r (10d6+10)*150%：骰(10d6+10)*150%，并输出其计算结果\n"
        u8"指令.r 4d6k3：骰4d6，取其中较高的三个结果\n"
        u8"指令.r 2d20kl1：骰2d20，取其中较低的一个结果\n"
        u8"指令.r 6#4d6kl3：骰6次4d6kl3，将它们的结果分别显示\n"
        u8"指令.r {1d20,1d12,1d10,1d8,1d6}：分别骰这些骰子，结果分别显示\n"
        u8"指令.r 2#d20-{1,2}：骰两次d20,结果分别减去1和2，结果分别显示\n"
        u8"指令.roll source off：停止显示你的骰子的详细结果\n"
        u8"指令.rs off：上述指令的较短形式\n"
        u8"指令.roll source on：恢复显示你的骰子的详细结果\n"
        u8"指令.rs on：上述指令的较短形式\n"
        u8"指令.rs 3d6：骰3d6，无视rs off状态，仍然显示详细结果";
}

static bool roll_source(std::string::const_iterator begin, std::string::const_iterator end, const event_info& event,
                        std::string& response, bool& detailed) {
    std::smatch detail_match;
    std::regex_match(begin, end, detail_match, std::regex("^(on|off) *"));
    if (!detail_match.empty()) {
        auto pfm = profile::profile_manager::get_instance();
        if (detail_match[1] == "on") {
            pfm->get_profile(event.user_id)->sys_vars[profile::sys_var_type::rs_on] = profile::var_rs_on;
            pfm->force_update(event.user_id);
            output_constructor oc(event.nickname);
            oc << u8"启用骰子详细输出";
            response = oc;
            return true;
        } else {
            pfm->get_profile(event.user_id)->sys_vars[profile::sys_var_type::rs_on] = profile::var_rs_off;
            pfm->force_update(event.user_id);
            output_constructor oc(event.nickname);
            oc << u8"关闭骰子详细输出";
            response = oc;
            return true;
        }
    } else {
        detailed = true;
        return false;
    }
};

static const std::regex filter_command("^s(?:ource)? *", std::regex_constants::icase);
static auto return_same = [](const std::string& s) -> std::string { return s; };

static bool request_with_except(std::string const& message, event_info& event, std::string& response) {
    bool detailed_roll = false;

    auto work_point = message.cbegin();
    auto end_point = message.cend();

    std::smatch match_cmd_detail;
    std::regex_search(work_point, end_point, match_cmd_detail, filter_command);

    auto pfm = profile::profile_manager::get_instance();

    if (!match_cmd_detail.empty()) {
        work_point += match_cmd_detail[0].length();
        if (roll_source(work_point, end_point, event, response, detailed_roll)) {
            return true;
        }
    } else {
        profile::var_pair var = pfm->get_profile(event.user_id)->sys_vars[profile::sys_var_type::rs_on];
        detailed_roll = static_cast<bool>(var.second);
    }

    std::string message_cp = std::string(work_point, end_point);
    std::string str_roll_command;
    std::string str_roll_detail;
    std::string str_tail;
    std::string str_result;

    using namespace diceparser;
    auto do_dicelet = [&str_result, &str_roll_detail, detailed_roll](diceparser::p_dicelet const& rpointer) -> bool {
        str_container strs_detail;
        result_container results;
        rpointer->roll_dicelet(results, strs_detail);
        str_result.append("{");
        std::for_each(results.begin(), results.end() - 1, [&str_result](auto& num) { str_result.append(num).append(", "); });
        str_result.append(results.back()).append("}");
        if (detailed_roll) str_roll_detail.assign(strs_detail.str());
        return true;
    };

    auto do_other = [&str_result, &str_roll_detail, detailed_roll](p_component const& rpointer) -> bool {
        str_container strs_detail;
        number result = rpointer->roll_the_dice(strs_detail);
        str_result.append(result);
        if (detailed_roll) str_roll_detail.assign(strs_detail.str());
        return true;
    };

    bool flag = result_brancher(
        message_cp, str_roll_command, str_tail, &pfm->get_profile(event.user_id)->mac_rolls, do_other, do_other, do_dicelet);
    if (!flag) {
        if (pfm->get_profile(event.user_id)->def_roll.get(profile::def_roll_type::def_roll, message_cp)
            != profile::profile_status::finished) {
            return false;
        }
        result_brancher(
            message_cp, str_roll_command, str_tail, &pfm->get_profile(event.user_id)->mac_rolls, do_other, do_other, do_dicelet);
        str_tail = std::string(work_point, end_point) + str_tail;
    }

    output_constructor oc(event.nickname, str_tail);
    if (detailed_roll)
        oc.append_roll(str_roll_command, str_roll_detail, str_result);
    else
        oc.append_roll(str_roll_command, "", str_result);
    response = oc;
    return true;
}

bool entry_roll_dice::resolve_request(std::string const& message, event_info& event, std::string& response) noexcept {
    return shoot_exceptions(request_with_except, message, event, response);
}
#include "./entry_profile.h"

#include <regex>

#include "../data/profile_manager.h"
#include "../utils/utils.h"
#include "./wrappers.h"

using namespace dicebot;
using namespace dicebot::entry;

#pragma region set roll
entry_set_roll::entry_set_roll() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "s(?:et)?";
    this->identifier_list = {"set", "s"};

    this->help_message =
        u8"设置骰子(.set或者.s)\n"
        u8"指令.set 1d100：设定默认骰子为1d100，"
        u8"如果.r后没有接任何骰子时，会骰默认骰\n"
        u8"指令.set 4d6k3 属性：设定一个名称为“属性”的骰子为4d6k3\n"
        u8"指令.r属性：如果.r后面是骰子名称，则骰该名称的骰子\n"
        u8"注意：骰子名称中不能包含+-*/和空格";
}

static bool set_request_with_except(std::string const& message, const event_info& event, std::string& response) {
    auto pfm = profile::profile_manager::get_instance();

    std::string message_cp = message;

    auto set_default = [pfm, &event, &response](std::string const& str_roll_command) -> bool {
        pfm->get_profile(event.user_id)->def_roll[profile::def_roll_type::def_roll] = str_roll_command;
        pfm->force_update(event.user_id);

        output_constructor oc(event.nickname);
        oc << u8"设置默认骰子指令: " << str_roll_command;
        response = oc;
        return true;
    };

    auto set_named = [pfm, &event, &response](std::string const& str_roll_command, std::string const& str_message) -> bool {
        size_t find_pos = str_message.find_first_of(illegal_identifier);
        if (find_pos != std::string::npos) throw invalid_macro();
        pfm->get_profile(event.user_id)->mac_rolls[str_message] = str_roll_command;
        pfm->force_update(event.user_id);

        output_constructor oc(event.nickname);
        oc << u8"设置指令: " << str_roll_command << u8" 为 " << str_message;
        response = oc;
        return true;
    };

    diceparser::tokenizer::token_container_t tk_cont;
    diceparser::tokenizer tknz(
        tk_cont, diceparser::tokenizer_flag{true, true}, message_cp, &(pfm->get_profile(event.user_id)->mac_rolls));
    diceparser::parser parser(tknz);
    auto p_syntax = parser.parse();
    if (!p_syntax) return false;
    auto p_comp = diceparser::build_component_from_syntax(p_syntax.get());
    if (!p_comp) return false;

    auto p_number = std::dynamic_pointer_cast<diceparser::comp_number>(p_comp);
    if (p_number) {
        diceparser::str_container cont;

        number result = p_number->roll_the_dice(cont);

        if (!parser.tail.empty()) {
            return set_named(result, parser.tail);
        } else {
            return set_default(result);
        }
    }

    diceparser::str_container cont;

    auto return_same = [](const std::string& s) -> std::string { return s; };

    auto p_dicelet = std::dynamic_pointer_cast<diceparser::dicelet>(p_comp);
    if (p_dicelet) {
        if (!parser.tail.empty()) return false;
        p_dicelet->print(cont);
        return set_default("(" + cont.str() + ")");
    }

    auto p_holder = std::dynamic_pointer_cast<diceparser::base_holder>(p_comp);
    if (p_holder) {
        p_comp->print(cont);
        if (!parser.tail.empty())
            return set_named("(" + cont.str() + ")", parser.tail);
        else
            return set_default("(" + cont.str() + ")");
    }

    p_comp->print(cont);
    if (!parser.tail.empty())
        return set_named("(" + cont.str() + ")", parser.tail);
    else
        return set_default("(" + cont.str() + ")");
}

bool entry_set_roll::resolve_request(std::string const& message, event_info& event, std::string& response) noexcept {
    return shoot_exceptions(set_request_with_except, message, event, response);
}
#pragma endregion

#pragma region list
static auto defr_msg = [](profile::user_profile::def_roll_map_t const& map, std::string const& head, output_constructor& out) {
    if (map.empty()) return;
    out << "\r\n" << head;
    for (auto const& pair : map) {
        out << pair.second;
    }
};

static auto macro_msg = [](profile::user_profile::mac_roll_map_t const& map, std::string const& message,
                           output_constructor& out) {
    if (map.empty()) return;
    for (auto const& pair : map) {
        if (!message.empty() && pair.first.find(message) == std::string::npos) continue;
        out << u8"\r\n> " << pair.first << u8" : " << pair.second;
    }
};

entry_list::entry_list() noexcept {
    this->is_stand_alone = true;
    this->identifier_regex = "l(?:ist)?";
    this->identifier_list = {"list", "l"};

    this->help_message =
        u8"显示已保存的骰子(.list或者.l)\n"
        u8"指令.list：显示所有保存的骰子\n"
        u8"指令.l：上述指令的简写形式\n"
        u8"指令.list test：显示所有保存的骰子中，名称带有“test”的";
}

bool entry_list::resolve_request(std::string const& message, event_info& event, std::string& response) noexcept {
    if (message.empty()) {
        auto p_profile = profile::profile_manager::get_instance()->get_profile(event.user_id);
        output_constructor oc(event.nickname);
        oc << u8"已设置下列骰子指令: ";
        defr_msg(p_profile->def_roll, u8"* 默认 : ", oc);
        macro_msg(p_profile->mac_rolls, "", oc);
        response = oc;
        return true;
    } else {
        const auto& macros = profile::profile_manager::get_instance()->get_profile(event.user_id)->mac_rolls;

        output_constructor oc(event.nickname);
        oc << u8"已设置如下包含 \"" << message << "\" 的骰子指令:";
        macro_msg(macros, message, oc);
        response = oc;
        return true;
    }
    return false;
}
#pragma endregion

#pragma region delete
entry_delete::entry_delete() noexcept {
    this->is_stand_alone = false;
    this->identifier_regex = "d(?:elete)?";
    this->identifier_list = {"d", "delete"};

    this->help_message =
        u8"删除已保存的骰子(.delete或者.d)\n"
        u8"指令.delete：删除所有保存的骰子\n"
        u8"指令.delete 力量：删除名称为“力量”的骰子\n"
        u8"注意：默认骰子是无法删除的";
}

bool entry_delete::resolve_request(std::string const& message, event_info& event, std::string& response) noexcept {
    auto pfm = profile::profile_manager::get_instance();
    if (message.empty()) {
        pfm->get_profile(event.user_id)->mac_rolls.clear();
        pfm->force_update(event.user_id);

        output_constructor oc(event.nickname);
        oc << u8"已删除所有骰子指令";
        response = oc;
        return true;
    } else {
        const auto& p_macro = pfm->get_profile(event.user_id)->mac_rolls;
        auto iter = p_macro.find(message);
        if (iter == p_macro.end()) return false;
        pfm->get_profile(event.user_id)->mac_rolls.erase(iter);
        pfm->force_update(event.user_id);

        output_constructor oc(event.nickname);
        oc << u8"已删除骰子指令: " << message;
        response = oc;
        return true;
    }
    return false;
}
#pragma endregion
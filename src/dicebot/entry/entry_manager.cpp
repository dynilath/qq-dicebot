#include "./entry_manager.h"

#include <memory>

#include "../utils/utils.h"

using namespace dicebot;
using namespace entry;

void entry_manager::register_dice(p_entry entry) noexcept {
    for (std::string const& s : entry->identifier_list) {
        auto iter = this->entry_cmd_map.find(s);
        if (iter != this->entry_cmd_map.end()) continue;

        this->entry_cmd_map.insert(entry_pair_t(s, entry));
    }
    entry_list.push_back(entry);
}

void entry_manager::finish_initialization() noexcept {
    auto entry_help = std::make_shared<entry::entry_help>();
    for (const p_entry& prot : this->entry_list) {
        entry_help->register_help(prot);
    }
    entry_help->generate_filter_command();
    this->register_dice(entry_help);

    std::ostringstream ostrs_stream;
    ostrs_stream << "^(";
    bool is_first = true;

    for (auto& item : this->entry_list) {
        if (is_first) {
            is_first = false;
        } else
            ostrs_stream << "|";
        ostrs_stream << item->identifier_regex;
    }
    ostrs_stream << ") *";
    this->regex_command = std::regex(ostrs_stream.str(), std::regex_constants::icase);
}

p_entry const entry_manager::get_entry(std::string command) const {
    utils::lower_case(command);
    auto iter = this->entry_cmd_map.find(command);
    if (iter == entry_cmd_map.cend())
        return nullptr;
    else
        return (*iter).second;
}

std::regex const* entry_manager::get_regex_command() const { return &(this->regex_command); }

entry_help::entry_help() noexcept {
    this->is_stand_alone = true;
    this->identifier_regex = "h(?:elp)?";
    this->identifier_list = {"help", "h"};
    this->help_message =
        u8"DiceBot by dynilath v1.7.1\n"
        u8"使用方式简介：\n"
        u8".r 1d8+1d6+3 普通骰子\n"
        u8".c coc骰子\n"
        u8".wo4 owod骰子\n"
        u8".wn4 nwod骰子\n"
        u8".f FATE骰子\n"
        u8"更多指令：name set list delete manual poker\n"
        u8"使用 .help [指令名称] 来获得对应指令的帮助\n"
        u8"\n"
        u8"访问项目主页http://github.com/dynilath/coolq-dicebot\n"
        u8"获得使用说明以及了解更多内容。\n"
        u8"如果在使用中遇到了bug，或者有什么功能建议，欢迎在项目主页通过issue反"
        u8"馈，感谢您的帮助。";
}

bool entry_help::register_help(p_entry v_entry) noexcept {
    this->entry_regex_list.push_back(v_entry->identifier_regex);
    for (const std::string& var : v_entry->identifier_list) {
        auto i = this->help_map.insert(help_pair_t(var, v_entry->help_message));
        if (!i.second) return false;
    }
    return true;
}

void entry_help::generate_filter_command() noexcept {
    std::ostringstream ostrs_stream;
    ostrs_stream << "^(";
    bool is_first = true;
    auto iter = this->entry_regex_list.cbegin();
    for (auto& item : this->entry_regex_list) {
        if (is_first) {
            is_first = false;
        } else
            ostrs_stream << "|";
        ostrs_stream << item;
    }
    ostrs_stream << ")";
    this->filter_command = std::regex(ostrs_stream.str(), std::regex_constants::icase);
}

bool entry_help::resolve_request(std::string const& message, event_info& event, std::string& response) noexcept {
    std::smatch m;
    std::regex_search(message, m, this->filter_command);

    if (m.empty()) {
        response.assign(this->help_message);
        return true;
    } else {
        auto iter = this->help_map.find(m[1]);
        if (iter != this->help_map.end()) {
            response.assign(iter->second);
            return true;
        } else
            return false;
    }
}

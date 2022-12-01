#pragma once

#include <map>
#include <memory>
#include <regex>
#include "./entry_base.h"

namespace dicebot {
    using p_entry = std::shared_ptr<entry::entry_base>;
    class entry_manager {
        using entry_map_t = std::map<std::string, p_entry>;
        using entry_pair_t = entry_map_t::value_type;

    private:
        entry_map_t entry_cmd_map;
        std::list<p_entry> entry_list;
        std::regex regex_command;

    public:
        void register_dice(p_entry entry) noexcept;
        void finish_initialization() noexcept;
        p_entry const get_entry(std::string command) const;
        std::regex const& get_regex_command() const;
    };
} // namespace dicebot

namespace dicebot::entry {
    class entry_help : public entry_base {
        using help_map_t = std::map<std::string, std::string>;
        using help_pair_t = help_map_t::value_type;

    private:
        help_map_t help_map;
        std::regex filter_command;
        std::list<std::string> entry_regex_list;
        std::string default_help_message;

    public:
        entry_help() noexcept;
        bool register_help(p_entry v_entry) noexcept;
        void generate_filter_command() noexcept;
        bool resolve_request(std::string const& message, event_info& event, std::string& response) noexcept override;
    };
} // namespace dicebot::entry
#pragma once

#include "./common.h"
#include "./protocol_base.h"

namespace dicebot{
    class protocol_manager{
        using protocol_map_t = std::map<std::string, protocol::p_protocol>;
        using protocol_pair_t = protocol_map_t::value_type;
    private:
        protocol_map_t protocol_map;
        std::list<std::string> protocol_regex_list;
        std::regex regex_command;
    public:
        void register_dice(protocol::p_protocol protocol);
        void create_command_regex();
        protocol::p_protocol get_protocol(std::string command) const;
        std::regex const * get_regex_command() const;
    };
}
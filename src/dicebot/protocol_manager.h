#pragma once

#include "./common.h"
#include "./protocol_base.h"

namespace dicebot{
    class protocol_manager{
        using protocol_map_t = std::map<std::string, protocol::p_protocol>;
        using protocol_pair_t = protocol_map_t::value_type;
    private:
        protocol_map_t protocol_cmd_map;
        std::list<protocol::p_protocol> protocol_list;
        std::regex regex_command;
    public:
        void register_dice(protocol::p_protocol protocol);
        void finish_initialization();
        protocol::p_protocol const get_protocol(std::string command) const;
        std::regex const * get_regex_command() const;
    };
}

namespace dicebot::protocol{
    class protocol_help :public protocol_base{
        using help_map_t = std::map<std::string,std::string>;
        using help_pair_t = help_map_t::value_type;
    private:
        help_map_t help_map;
        std::regex filter_command;
        std::list<std::string> protocol_regex_list;
        std::string default_help_message;
    public:
        protocol_help();
        bool register_help(p_protocol v_protocol);
        void generate_filter_command();
        bool resolve_request(
            std::string const & message,
            event_info & event,
            std::string & response) override;
    };
}
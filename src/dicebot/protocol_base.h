#pragma once

#include "./common.h"

namespace dicebot::protocol{

    using dice_request = std::string * (
        std::string const &,
        event_info const &,
        std::string &);

    class output_constructor{
    private:
        ostrs protocol_ot;
    public:
        output_constructor(std::string const & nickname){
            protocol_ot.str("");
            protocol_ot << " * " << nickname;
        }

        void append_message(std::string const & message){
            protocol_ot << " " << message;
        }
        void append_message(int const & message){
            protocol_ot << " " << message;
        }
        void append_roll(std::string const & roll_command, std::string const & detail, std::string const & roll_result){
            protocol_ot << " 掷骰: " << roll_command;
            if(detail.size() > 0) protocol_ot << " = " << detail;
            protocol_ot<<" = " << roll_result;
        }

        void append_roll(std::string const & roll_command, std::string const & detail, int const roll_result){
            protocol_ot << " 掷骰: " << roll_command;
            if(detail.size() > 0) protocol_ot << " = " << detail;
            protocol_ot<<" = " << roll_result;
        }

        std::string str() noexcept{
            return protocol_ot.str();
        }
    };

    class protocol_base{
    public:
        bool is_stand_alone = false;
        std::string identifier_regex;
        std::list<std::string> identifier_list;
        std::string help_message;
        virtual bool resolve_request(
            std::string const & message,
            event_info & event,
            std::string & response){ return false; }
    };

    using p_protocol = std::shared_ptr<protocol_base>;
}

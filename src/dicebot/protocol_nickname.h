#pragma once

#include "./common.h"
#include "./protocol_base.h"
namespace dicebot::protocol{
    class protocol_nickname :public protocol_base{
    private:
        std::regex filter_command;
    public:
        protocol_nickname();
        bool resolve_request(
            std::string const & message,
            event_info & event,
            std::string & response) override;
    };
}

#pragma once

#include "./common.h"
#include "./protocol_base.h"

namespace dicebot::protocol{
    class protocol_wod_dice : public protocol_base{
        using wod_call = bool(protocol_wod_dice::* )(std::string const &, std::string const &, std::string &);
        using wod_map = std::map<std::string, wod_call>;
        using wod_pair = wod_map::value_type;
    private:
        wod_map method_map;
        std::regex full_dice;
        std::regex filter_command;
    public:
        protocol_wod_dice();
        bool resolve_request(
            std::string const & message,
            event_info & event,
            std::string & response) override;

        bool nwod(std::string const & message, std::string const & nick_name, std::string & response);
        bool owod(std::string const & message, std::string const & nick_name, std::string & response);
    };

    class protocol_coc_dice : public protocol_base{
    private:
        std::regex full_dice;
    public:
        protocol_coc_dice();
        bool resolve_request(
            std::string const & message,
            event_info & event,
            std::string & response) override;
    };

    class protocol_fate_dice : public protocol_base{
        std::regex full_dice;
    public:
        protocol_fate_dice();
        bool resolve_request(
            std::string const & message,
            event_info & event,
            std::string & response) override;
    };
}
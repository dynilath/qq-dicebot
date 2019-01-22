#pragma once

#include "./common.h"
#include "./protocol_base.h"
#include <functional>

namespace dicebot::protocol{
    class protocol_manual_dice : public protocol_base{
        using manual_dice_call = std::function<bool(
            protocol_manual_dice const &, 
            std::string const &, 
            std::string const &, 
            std::string &, 
            int64_t const, int64_t const)>;

        using manual_map = std::map<std::string, manual_dice_call>;
        using manual_pair = manual_map::value_type;
    private:
        manual_map method_map;
    public:
        std::regex filter_manual_dice;
        std::regex filter_integer;
        std::regex filter_command;
        protocol_manual_dice();
        bool resolve_request(
            std::string const & message,
            event_info & event,
            std::string & response) override;
    };

    
    #define TMDICE_CALL(_Func_Name) \
    bool _Func_Name(\
    protocol_manual_dice const & protocol_cal, \
    std::string const & message, \
    std::string const & nick_name, \
    std::string & response,\
    int64_t const group_id, int64_t const user_id)

    #define TMDICE_CALL_NAMESPACE(_Func_Name) \
    bool protocol::_Func_Name(\
    protocol_manual_dice const & protocol_cal, \
    std::string const & message, \
    std::string const & nick_name, \
    std::string & response,\
    int64_t const group_id, int64_t const user_id)

    TMDICE_CALL(manualdice_add);
    TMDICE_CALL(manualdice_killall);
    TMDICE_CALL(manualdice_kill);
    TMDICE_CALL(manualdice_roll);
    TMDICE_CALL(manualdice_create);
}
#include "./protocol_manual_dice.h"


#include "./dice_roller.h"
#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./manual_dice.h"
#include "./manual_dice_control.h"
#include "../cqsdk/utils/base64.h"

namespace base64 = cq::utils::base64;
using namespace dicebot;
using namespace dicebot::protocol;

protocol_manual_dice::protocol_manual_dice(){
    this->is_stand_alone = false;
    this->filter_manual_dice = std::regex("^ *((\\+)?\\d*d\\d+)(\\+\\d*d\\d+)* *",std::regex_constants::icase);
    this->filter_integer = std::regex("^ *(\\d+) *");
    this->filter_command = std::regex("^(ka|a|k|r) *",std::regex_constants::icase);

    this->identifier_regex = "m(?:annual)?";
    this->identifier_list = {"mannual","m"};


    this->method_map.insert(manual_pair("ka", &manualdice_killall));
    this->method_map.insert(manual_pair("k", &manualdice_kill));
    this->method_map.insert(manual_pair("a", &manualdice_add));
    this->method_map.insert(manual_pair("r", &manualdice_roll));

    this->help_message = base64::decode(
        "5omL5Yqo6aqw5a2QKC5tYW51YWzmiJbogIUubSkK"
        "5oyH5LukLm0gW2RpY2Vd77ya5Lqn55Sf5omL5Yqo"
        "6aqw5a2Q77yM5ZCO5o6l6aqw5a2Q77yM6L+Z6YeM"
        "5LiN5pSv5oyB566X5byPCuaMh+S7pC5tciBbbnVt"
        "YmVyXe+8mumqsOaMh+WumumqsOWtkO+8jOWQjuaO"
        "pemqsOWtkOW6j+WPtwrmjIfku6QubWsgW251bWJl"
        "cl3vvJrmtojnga3mjIflrprpqrDlrZDvvIzlkI7m"
        "jqXpqrDlrZDluo/lj7cK5oyH5LukLm1rYe+8mua4"
        "heepuuaJgOacieaJi+WKqOmqsOWtkA==");
}

bool protocol_manual_dice::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){
    std::string str_nickname = event.nickname;

    std::smatch match_command;
    std::regex_search(message, match_command, filter_command);
    if(match_command.size() == 0)
        return manualdice_create(*this,message, str_nickname, response, event.group_id, event.user_id);

    std::string str_match = match_command[1];
    std::transform(str_match.begin(),str_match.end(),str_match.begin(),tolower);

    auto iter = this->method_map.find(str_match);
    if(iter != method_map.end()){
        manual_dice_call dice_call  = (*iter).second;
        return dice_call(*this,match_command.suffix().str(), str_nickname,response, event.group_id, event.user_id);
    }
    return false;
}

TMDICE_CALL_NAMESPACE(manualdice_add){
    std::smatch roll_match;
    std::regex_search(message, roll_match, protocol_cal.filter_manual_dice);
    if(roll_match.size() > 0){
        std::string str_command = roll_match.str();
        std::string str_roll_message = roll_match.suffix().str();
        utils::remove_space_and_tab(str_command);
        manual::p_manual md_manualdice = manual::manual_dice_control::instance->add_manual_dice(user_id, group_id, str_command);
        if(md_manualdice && (*md_manualdice)){
            ostrs ostr(ostrs::ate);
            ostr << u8" * " << nick_name;
            if(str_roll_message.size() > 0) ostr << u8" " << str_roll_message;
            ostr << u8" 在桌上增加了这些骰子: " << str_command;
            ostr << u8" 当前状态: " << md_manualdice->str();
            response = ostr.str();
            return true;
        }
    }
    return false;
}

TMDICE_CALL_NAMESPACE(manualdice_killall){
    manual::p_manual md_manualdice = manual::manual_dice_control::instance->killall_manual_dice(user_id, group_id);
    if(md_manualdice && (*md_manualdice)){
        ostrs ostr(ostrs::ate);
        ostr << u8" * " << nick_name;
        if(message.size() > 0) ostr << u8" " << message;
        ostr << u8" 杀掉了所有的骰子 ";
        ostr << u8"当前状态: " << md_manualdice->str();
        response = ostr.str();
        return true;
    }
    return false;
}

TMDICE_CALL_NAMESPACE(manualdice_kill){
    std::smatch roll_match;
    std::regex_search(message, roll_match, protocol_cal.filter_integer);
    if(roll_match.size() > 0){
        std::string str_command = roll_match.str();
        std::string str_roll_message = roll_match.suffix().str();
        utils::remove_space_and_tab(str_command);

        manual::p_manual md_manualdice = manual::manual_dice_control::instance->kill_manual_dice(user_id, group_id, str_command);
        if(md_manualdice && (*md_manualdice)){
            ostrs ostr(ostrs::ate);
            ostr << u8" * " << nick_name;
            if(str_roll_message.size() > 0) ostr << u8" " << str_roll_message;
            ostr << u8" 杀死桌上的第 " << str_command << u8" 个骰子 ";
            ostr << u8"当前状态: " << md_manualdice->str();
            response = ostr.str();
            return true;
        }
    }
    return false;
}

TMDICE_CALL_NAMESPACE(manualdice_roll){
    std::smatch roll_match;
    std::regex_search(message, roll_match, protocol_cal.filter_integer);
    if(roll_match.size() > 0){
        std::string str_command = roll_match.str();
        std::string str_roll_message = roll_match.suffix().str();
        utils::remove_space_and_tab(str_command);

        manual::p_manual md_manualdice = manual::manual_dice_control::instance->roll_manual_dice(user_id, group_id, str_command);
        if(md_manualdice && (*md_manualdice)){
            ostrs ostr(ostrs::ate);
            ostr << u8" * " << nick_name;
            if(str_roll_message.size() > 0) ostr << u8" " << str_roll_message;
            ostr << u8" 重骰桌上的第 " << str_command << u8" 个骰子 ";
            ostr << u8"当前状态: " << md_manualdice->str();
            response = ostr.str();
            return true;
        }
    }
    return false;
}

TMDICE_CALL_NAMESPACE(manualdice_create){
    std::smatch roll_match;
    std::regex_search(message, roll_match, protocol_cal.filter_manual_dice);
    if(roll_match.size() > 0){
        std::string str_command = roll_match.str();
        std::string str_roll_message = roll_match.suffix().str();
        utils::remove_space_and_tab(str_command);

        manual::p_manual md_manualdice = manual::manual_dice_control::instance->create_manual_dice(user_id, group_id, str_command);
        if(md_manualdice && (*md_manualdice)){
            ostrs ostr(ostrs::ate);
            ostr << u8" * " << nick_name;
            if(str_roll_message.size() > 0) ostr << u8" " << str_roll_message;
            ostr << u8" 在桌上放了这些骰子: " << str_command;
            ostr << u8" 当前状态: " << md_manualdice->str();
            response = ostr.str();
            return true;
        }
    }
    return false;
}

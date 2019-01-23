#include "./protocol_nickname.h"

#include "./nick_manager.h"

using namespace dicebot;
using namespace dicebot::protocol;

protocol_nickname::protocol_nickname(){
    this->is_stand_alone = false;
    this->filter_command = std::regex("^(s)(?:ilence)? *",std::regex_constants::icase);
    this->identifier_regex = "n(?:ame)?";
    this->identifier_list = {"name","n"};
}

bool protocol_nickname::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){

    bool is_silence = false;
    std::smatch match_list_silence;
    std::regex_search(message, match_list_silence, this->filter_command);


    if(match_list_silence.size() == 0){
        std::string message_cp;
        int pos_t = message.find_first_not_of(" \t");
        if(pos_t != std::string ::npos) message_cp.assign(message.substr(pos_t));
        else message_cp.assign(message);

        std::string str_origin_name = event.nickname;

        if(message_cp.length() > 0){
            event.nickname = message_cp;
            (nickname::nickname_manager::instance)->set_nickname(event);

            ostrs ot(ostrs::ate);
            ot << u8" * " << str_origin_name << u8" 的新名字是 " << event.nickname;
            response = ot.str();
            return true;
        }
        else{
            ostrs ot(ostrs::ate);
            ot << u8" * " << str_origin_name << u8" 的名字是 " << event.nickname;
            response = ot.str();
            return true;
        }
    }
    else{
        std::string s_message = match_list_silence.suffix().str();
        int pos_t = s_message.find_first_not_of(" \t");
        if(pos_t != std::string::npos) s_message = s_message.substr(pos_t);

        if(s_message.length() > 0){
            event.nickname = s_message;
            (nickname::nickname_manager::instance)->set_nickname(event);
        }
    }
    return false;
}
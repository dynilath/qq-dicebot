#include "./protocol_specialized_dice.h"
#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./dice_roller.h"
#include "../cqsdk/utils/base64.h"

namespace base64 = cq::utils::base64;
using namespace dicebot;
using namespace dicebot::protocol;

#pragma region wod
protocol_wod_dice::protocol_wod_dice(){
    this->is_stand_alone = false;
    this->full_dice = std::regex("^(\\d+)(?:d(\\d+))?(?:b(\\d+))? *",std::regex_constants::icase);
    this->filter_command = std::regex("^(n|o) *",std::regex_constants::icase);

    this->identifier_regex = "w(?:od)?";
    this->identifier_list = {"wod","w"};
    this->method_map.insert(std::pair<std::string, wod_call>("n", &protocol_wod_dice::nwod));
    this->method_map.insert(std::pair<std::string, wod_call>("o", &protocol_wod_dice::owod));

    this->help_message = base64::decode(
        "V29E5a6a5Yi26aqw5a2QKC53b2TmiJYudykK5oyH"
        "5LukLndvNO+8muS9v+eUqG93b2Top4TliJnov5vo"
        "oYzpqrDmsaDkuLo055qE5qOA5a6aCuaMh+S7pC53"
        "bjTvvJrkvb/nlKhud29k6KeE5YiZ6L+b6KGM6aqw"
        "5rGg5Li6NOeahOajgOWumgrmjIfku6Qud280ZDjv"
        "vJrmjIflrprpmr7luqbkuLo4CuaMh+S7pC53bjRi"
        "OO+8muaMh+WumuWcqDjmiJbku6XkuIrojrflvpfl"
        "pZblirHpqrA=");

}

bool protocol_wod_dice::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){

    std::smatch command_match;
    std::regex_search(message, command_match, this->filter_command);
    if(command_match.size() == 0)
        return false;

    std::string str_match = command_match[1];
    std::transform(str_match.begin(),str_match.end(),str_match.begin(),tolower);

    auto iter = this->method_map.find(str_match);
    if(iter != method_map.end()){
        wod_call dice_call = (*iter).second;
        return (this->*dice_call)(command_match.suffix().str(), event.nickname,response);
    }

    return false;
}

bool protocol_wod_dice::nwod(std::string const & message, std::string const & nick_name, std::string & response){
    std::smatch command_match;
    ostrs ostr(ostrs::ate);
    std::regex_search(message, command_match, this->full_dice);
    if(command_match.size() > 0){
        std::string str_roll_msg = command_match.suffix().str();
        std::string str_roll_source = command_match.str();
        utils::remove_space_and_tab(str_roll_source);

        roll::dice_roll dr;    
        roll::roll_nwod(dr,str_roll_source);
        if(dr){
            
            output_constructor oc(nick_name);
            if(str_roll_msg.size() > 0) oc.append_message(str_roll_msg);
            oc.append_roll("nWoD", dr.detail(), dr.summary);
            response = oc.str();

            return true;
        }
    }
    return false;
}

bool protocol_wod_dice::owod(std::string const & message, std::string const & nick_name, std::string & response){
    std::smatch command_match;
    ostrs ostr(ostrs::ate);
    std::regex_search(message, command_match, this->full_dice);
    if(command_match.size() > 0){
        std::string str_roll_msg = command_match.suffix().str();
        std::string str_roll_source = command_match.str();
        utils::remove_space_and_tab(str_roll_source);

        roll::dice_roll dr;    
        roll::roll_owod(dr,str_roll_source);
        if(dr){
            
            output_constructor oc(nick_name);
            if(str_roll_msg.size() > 0) oc.append_message(str_roll_msg);
            oc.append_roll("oWoD", dr.detail(), dr.summary);
            response = oc.str();

            return true;
        }
    }
    return false;
}
#pragma endregion

#pragma region coc
protocol_coc_dice::protocol_coc_dice(){
    this->is_stand_alone = false;
    this->full_dice = std::regex("^([pb]\\d+ *)* *", std::regex_constants::icase);
    this->identifier_regex = "c(?:oc)?";
    this->identifier_list ={"coc","c"};    
    
    this->help_message = base64::decode(
        "Q29D5a6a5Yi26aqw5a2QKC5jb2PmiJYuYykK5oyH"
        "5LukLmNvY++8mmNvY+WumuWItumqsOWtkArmjIfk"
        "u6QuY++8muS4iui/sOWRveS7pOeahOeugOWGmeW9"
        "ouW8jwrmjIfku6QuY29jIHAx77ya5oOp572a6aqw"
        "Me+8iHBlbmFsdHkgMe+8iQrmjIfku6QuY29jIGIx"
        "77ya5aWW5Yqx6aqwMe+8iGJvbnVzIDHvvIk=");
}

bool protocol_coc_dice::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){
    std::smatch roll_match;
    std::regex_search(message, roll_match, full_dice);
    if(roll_match.size() > 0){
        std::string str_roll_message = roll_match.suffix().str();
        std::string str_roll_source = roll_match.str();
        dicebot::utils::remove_space_and_tab(str_roll_source);

        //roll::dice_roller diceRoll(str_roll_source, roll::roll_mode::COC_PB);
        roll::dice_roll dr;
        roll::roll_coc(dr,str_roll_source);
        if(dr){
            output_constructor oc(event.nickname);
            if(str_roll_message.size() > 0) oc.append_message(str_roll_message);
            oc.append_roll("CoC " + str_roll_source, dr.detail_coc(), dr.summary);
            response = oc.str();

            return true;
        }
    }
    return false;
}
#pragma endregion

#pragma region fate
protocol_fate_dice::protocol_fate_dice(){
    this->is_stand_alone = false;
    this->full_dice = "^([\\+|\\-]\\d+)? *";
    this->identifier_regex = "f(?:ate)?";
    this->identifier_list ={"fate","f"};

    this->help_message = base64::decode(
        "RkFUReWumuWItumqsOWtkCguZmF0ZeaIli5mKQrm"
        "jIfku6QuZu+8mkZBVEXpqrAK5oyH5LukLmYrNO+8"
        "muaMh+Wumis05L+u5q2j");
}

bool protocol_fate_dice::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){

    std::smatch roll_match;
    std::regex_search(message, roll_match, full_dice);

    std::string str_roll_message;

    roll::dice_roll dr;

    if(roll_match[1].matched){
        std::string str_command = roll_match[1];
        str_roll_message = roll_match.suffix();
        roll::roll_fate(dr,str_command);
    }
    else{
        str_roll_message = message;
        roll::roll_fate(dr);
    }

    if(dr){
        output_constructor oc(event.nickname);
        if(str_roll_message.size() > 0) oc.append_message(str_roll_message);
        oc.append_roll("FATE", dr.detail_fate(), dr.summary);
        response = oc.str();
        return true;
    }
    return false;
}
#pragma endregion
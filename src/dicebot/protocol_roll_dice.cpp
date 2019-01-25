#include "./protocol_roll_dice.h"

#include "./dice_utils.h"
#include "./nick_manager.h"
#include "./dice_spliter.h"
#include "./profile_manager.h"
#include "../cqsdk/utils/base64.h"

//regular dice, with detailed info

namespace base64 = cq::utils::base64;
using namespace dicebot;
using namespace dicebot::protocol;

protocol_roll_dice::protocol_roll_dice(){
    this->is_stand_alone = false;
    this->filter_command = std::regex("^s(?:ource)? *", std::regex_constants::icase);
    this->identifier_regex = "r(?:oll)?";
    this->identifier_list = {"roll","r"};
    this->help_message = base64::decode(
        "6aqw5a2Q5oyH5LukKC5yb2xs5oiWLnIpCuaMh+S7"
        "pC5yIDFkMjDvvJrpqrDkuIDkuKoxZDIwCuaMh+S7"
        "pC5yIDFkMjArMmQ2KzTvvJrpqrAxZDIwKzJkNis0"
        "77yM5rGC5YW257uT5p6cCuaMh+S7pC5yICgxMGQ2"
        "KzEwKSoxNTAl77ya6aqwKDEwZDYrMTApKjE1MCXv"
        "vIzmsYLlhbbnu5PmnpwK5oyH5LukLnIgNGQ2azPv"
        "vJrpqrA0ZDbvvIzlj5blhbbkuK3ovoPpq5jnmoTk"
        "uInkuKrnu5PmnpwK5oyH5LukLnIgMmQyMGtsMe+8"
        "mumqsDJkMjDvvIzlj5blhbbkuK3ovoPkvY7nmoTk"
        "uIDkuKrnu5PmnpwK5oyH5LukLnJvbGwgc291cmNl"
        "IG9mZu+8muWBnOatouaYvuekuuS9oOeahOmqsOWt"
        "kOeahOivpue7hue7k+aenArmjIfku6QucnMgb2Zm"
        "77ya5LiK6L+w5oyH5Luk55qE6L6D55+t5b2i5byP"
        "CuaMh+S7pC5yb2xsIHNvdXJjZSBvbu+8muaBouWk"
        "jeaYvuekuuS9oOeahOmqsOWtkOeahOivpue7hue7"
        "k+aenArmjIfku6QucnMgb27vvJrkuIrov7DmjIfk"
        "u6TnmoTovoPnn63lvaLlvI8K5oyH5LukLnJzIDNk"
        "Nu+8mumqsDNkNu+8jOaXoOinhnJzIG9mZuaViOae"
        "nO+8jOS7jeeEtuaYvuekuuivpue7hue7k+aenA=="
        );
}

bool protocol_roll_dice::resolve_request(
        std::string const & message,
        event_info & event,
        std::string & response){
    bool detailed_roll_message = false;
    std::smatch match_list_command_detail;
    std::regex_search(message, match_list_command_detail, this->filter_command);
    
    std::string message_cp;
    profile::profile_manager * pfm = profile::profile_manager::get_instance();

    profile::var_pair var;
    
    if(pfm->get_value<profile::sys_var_type,profile::var_pair>(
        profile::sys_var_type::rs_on,var,event.user_id)){
            if(var.second) detailed_roll_message = true;
            else detailed_roll_message = false;
    }

    if(match_list_command_detail.size() > 0){
        message_cp = match_list_command_detail.suffix().str();
        if(message_cp == "on"){
            var.second = var.first;
            if(pfm->set_value<profile::sys_var_type,profile::var_pair>(
                profile::sys_var_type::rs_on,var,event.user_id)){
                output_constructor oc(event.nickname);
                oc.append_message(u8"启用骰子详细输出");
                response = oc.str();
                return true;
            }
            else return false;
        }
        else if(message_cp == "off"){
            var.second = !var.first;
            if(pfm->set_value<profile::sys_var_type,profile::var_pair>(
                profile::sys_var_type::rs_on,var,event.user_id)){
                output_constructor oc(event.nickname);
                oc.append_message(u8"关闭骰子详细输出");
                response = oc.str();
                return true;
            }
            else return false;
        }
        else{
            detailed_roll_message = true;
        }
    }
    else message_cp = message;

    std::string str_roll_command;
    std::string str_roll_detail;
    std::string str_result;
    std::string str_message;

    binary_tree_split_dice(message_cp, str_roll_command, str_roll_detail, str_result, str_message);

    if(str_roll_command.size() > 0){
        output_constructor oc(event.nickname);
        if(str_message.size()>0) oc.append_message(str_message);
        if(detailed_roll_message) oc.append_roll(str_roll_command,str_roll_detail,str_result);
        else oc.append_roll(str_roll_command,"",str_result);
        response = oc.str();
        return true;
    }
    else{
        std::string saved_roll;
        if(message_cp.size() > 0){
            std::string message_for;

            size_t p;
            if(utils::find_space(message_cp,p)){
                if(p < message_cp.length()) message_for = message_cp.substr(p+1);
                message_cp = message_cp.substr(0,p);
            }

            if(pfm->get_value<std::string,std::string>(
                message_cp, saved_roll, event.user_id)){
                binary_tree_split_dice(saved_roll, str_roll_command, str_roll_detail, str_result, str_message);

                output_constructor oc(event.nickname);
                if(message_for.size()>0) oc.append_message(message_for);
                if(detailed_roll_message) oc.append_roll(str_roll_command,str_roll_detail,str_result);
                else oc.append_roll(str_roll_command,"",str_result);
                response = oc.str();
                return true;
            }
        }
        
        if(pfm->get_value<profile::def_roll_type,std::string>(
            profile::def_roll_type::def_roll,saved_roll,event.user_id)){
            binary_tree_split_dice(saved_roll, str_roll_command, str_roll_detail, str_result, str_message);

            output_constructor oc(event.nickname);
            if(message_cp.size()>0) oc.append_message(message_cp);
            if(detailed_roll_message) oc.append_roll(str_roll_command,str_roll_detail,str_result);
            else oc.append_roll(str_roll_command,"",str_result);
            response = oc.str();
            return true;
        }
    }
    return false;
}
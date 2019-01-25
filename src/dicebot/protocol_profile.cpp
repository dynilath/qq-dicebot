#include "./protocol_profile.h"

#include "./dice_utils.h"
#include "./profile_manager.h"
#include "./dice_spliter.h"
#include "./nick_manager.h"
#include "./profile_manager.h"
#include "../cqsdk/utils/base64.h"

namespace base64 = cq::utils::base64;
using namespace dicebot;
using namespace dicebot::protocol;

#pragma region set roll
protocol_set_roll::protocol_set_roll(){
    this->is_stand_alone = false;
    this->identifier_regex = "s(?:et)?";
    this->identifier_list = {"set","s"};
    this->filter_name = "^([^\\+\\-\\*/\\(\\)\\s]+)";

    this->help_message = base64::decode(
        "6K6+572u6aqw5a2QKC5zZXTmiJbogIUucykK5oyH"
        "5LukLnNldCAxZDEwMO+8muiuvuWumum7mOiupOmq"
        "sOWtkOS4ujFkMTAwCuaMh+S7pC5y77ya5aaC5p6c"
        "LnLmsqHmnInmjqXku7vkvZXpqrDlrZDml7bvvIzk"
        "vJrpqrDpu5jorqTpqrAK5oyH5LukLnNldCA0ZDZr"
        "MyDlsZ7mgKfvvJrorr7lrprkuIDkuKrlkI3np7Dk"
        "uLrigJzlsZ7mgKfigJ3nmoTpqrDlrZDkuLo0ZDZr"
        "MwrmjIfku6QucuWxnuaAp++8muWmguaenC5y5ZCO"
        "6Z2i5piv6aqw5a2Q5ZCN56ew77yM5YiZ6aqw6K+l"
        "5ZCN56ew55qE6aqw5a2QCuazqOaEj++8mumqsOWt"
        "kOWQjeensOS4reS4jeiDveWMheWQqystKi/lkozn"
        "qbrmoLw=");
}

bool protocol_set_roll::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){

    profile::profile_manager * pfm = profile::profile_manager::get_instance();

    std::string message_cp = message;

    std::string str_roll_command;
    std::string str_roll_detail;
    std::string str_result;
    std::string str_message;

    binary_tree_split_dice(message_cp, str_roll_command, str_roll_detail, str_result, str_message);
    if(str_result.size() == 0) return false;
    
    if(str_message.size() > 0){
        std::smatch m_name;
        std::regex_search(str_message,m_name,this->filter_name);
        if(!m_name[1].matched) return false;
        str_message = m_name[1];

        pfm->set_value<std::string,std::string>(
            str_message,str_roll_command,event.user_id);
        
        output_constructor oc(event.nickname);
        oc.append_message(u8"设置骰子指令:");
        oc.append_message(str_roll_command);
        oc.append_message(u8"为");
        oc.append_message(str_message);
        response = oc.str();
        return true;
    }
    else{
        pfm->set_value<profile::def_roll_type,std::string>(
            profile::def_roll_type::def_roll,str_roll_command,event.user_id);

        output_constructor oc(event.nickname);
        oc.append_message(u8"设置默认骰子指令:");
        oc.append_message(str_roll_command);
        response = oc.str();
        return true;
    }
    return false;
}
#pragma endregion

#pragma region set var
protocol_set_var::protocol_set_var(){
    this->is_stand_alone = false;
    this->filter_var = std::regex("^ *(\\+|-)? *(\\d+) *");
    this->filter_command = std::regex("^ *reset");
    this->identifier_regex = "v(?:ar)?";
    this->identifier_list ={"var","v"};
    this->filter_name = "^([^\\+\\-\\*/\\(\\)\\s]+)";

    this->help_message = base64::decode(
        "6K6+572u5Y+Y6YePKC52YXLmiJbogIUudikK5oyH"
        "5LukLnYgMTIg55Sf5ZG977ya6K6+5a6a5LiA5Liq"
        "5ZCN56ew5Li64oCc55Sf5ZG94oCd55qE5Y+Y6YeP"
        "77yM5Yid5aeL5YC85Li6MTIK5oyH5LukLnYgLTQg"
        "55Sf5ZG977ya5L+u5pS55ZCN56ew5Li64oCc55Sf"
        "5ZG94oCd55qE5Y+Y6YeP77yM5YW25pWw5YC8LTQK"
        "5oyH5LukLnYgKzQg55Sf5ZG977ya5L+u5pS55ZCN"
        "56ew5Li64oCc55Sf5ZG94oCd55qE5Y+Y6YeP77yM"
        "5YW25pWw5YC8KzQK5oyH5LukLnYgcmVzZXTvvJrp"
        "h43nva7miYDmnInlj5jph4/kuLrliJ3lp4vlgLw=");
}

bool protocol_set_var::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){

    profile::profile_manager * pfm = profile::profile_manager::get_instance();

    std::string message_cp;

    std::smatch m_cmd;
    std::regex_search(message,m_cmd,filter_command);
    if(m_cmd.size() > 0){
        if(pfm->reset_var(event.user_id)){
            output_constructor oc(event.nickname);
            oc.append_message(u8"重置所有值。");
            response = oc.str(); 
            return true;
        }
        return false;
    }

    std::smatch m;
    std::regex_search(message,m,filter_var);
    if(m.size() == 0) return false;
    
    if(m[1].matched){
        std::string s_oper = m[1];
        int oper =  1;
        if(s_oper[0]=='-') oper = -1;

        profile::var_pair val;

        message_cp = m.suffix();
        if(message_cp.length() == 0) return false;

        int mod_val = stoi(m[2]);

        if(pfm->get_value<std::string,profile::var_pair>(message_cp,val,event.user_id)){
            val.second += oper * mod_val;
            pfm->set_value<std::string,profile::var_pair>(message_cp,val,event.user_id);
            
            output_constructor oc(event.nickname);
            oc.append_message(u8"修改:");
            oc.append_message(message_cp);
            oc.append_message(u8"值为");
            oc.append_message(val.second);

            response = oc.str(); 
            return true;
        }
    }
    else{
        int i_val = stoi(m[2]);
        profile::var_pair val(i_val,i_val);

        std::smatch m_name;
        message_cp = m.suffix();
        std::regex_search(message_cp,m_name,this->filter_name);
        if(!m_name[1].matched) return false;
        message_cp = m_name[1];

        pfm->set_value<std::string,profile::var_pair>(message_cp,val,event.user_id);
            
        output_constructor oc(event.nickname);
        oc.append_message(u8"设置:");
        oc.append_message(message_cp);
        oc.append_message(u8"值为");
        oc.append_message(val.second);

        response = oc.str();
        return true;
    }
    return false;
}
#pragma endregion

#pragma region list
protocol_list::gen_var_t protocol_list::var_msg = [](
    profile::user_profile::user_var_map_type const & map,
    std::string const & head,
    std::string const & message,
    output_constructor & out){
        if(map.size() == 0) return;
        out.append_message("\r\n" + head);
        for(auto const & pair : map){
            if(message.size() > 0 && pair.first.find(message) == std::string::npos) continue;
            out.append_message(u8"\r\n>");
            out.append_message(pair.first);
            out.append_message(u8":");
            out.append_message(pair.second.second);
            out.append_message(u8"/");
            out.append_message(pair.second.first);
        }
    };

protocol_list::gen_defr_t protocol_list::defr_msg  = [](
    profile::user_profile::def_roll_map_type const & map,
    std::string const & head,
    output_constructor & out){
        if(map.size() == 0) return;
        out.append_message("\r\n" + head);
        for(auto const & pair : map){
            out.append_message(u8"\r\n>");
            out.append_message(pair.second);
        }
    };    

protocol_list::gen_macro_t protocol_list::macro_msg = [](
    profile::user_profile::mac_roll_map_type const & map,
    std::string const & head,
    std::string const & message,
    output_constructor & out){
        if(map.size() == 0) return;
        out.append_message("\r\n" + head);
        for(auto const & pair : map){
            if(message.size() > 0 && pair.first.find(message) == std::string::npos) continue;
            out.append_message(u8"\r\n>");
            out.append_message(pair.first);
            out.append_message(u8":");
            out.append_message(pair.second);
        }
    };

protocol_list::protocol_list(){
    this->is_stand_alone = true;
    this->filter_command = std::regex("^(all|v(?:ar)?|r(?:oll)?) *");
    this->identifier_regex = "l(?:ist)?";
    this->identifier_list ={"list","l"};

    this->help_message = base64::decode(
        "5pi+56S65bey5L+d5a2Y55qE6aqw5a2Q5ZKM5Y+Y"
        "6YePKC5saXN05oiW6ICFLmwpCuaMh+S7pC5saXN0"
        "IHJvbGzvvJrmmL7npLrmiYDmnInkv53lrZjnmoTp"
        "qrDlrZAK5oyH5LukLmxy77ya5LiK6L+w5oyH5Luk"
        "55qE566A5YaZ5b2i5byPCuaMh+S7pC5saXN0IHZh"
        "cu+8muaYvuekuuaJgOacieS/neWtmOeahOWPmOmH"
        "jwrmjIfku6QubHbvvJrkuIrov7DmjIfku6TnmoTn"
        "roDlhpnlvaLlvI8K5oyH5LukLmxpc3Qgcm9sbCDl"
        "ipvvvJrmmL7npLrmiYDmnInkv53lrZjnmoTpqrDl"
        "rZDkuK3vvIzlkI3np7DluKbmnInigJzlipvigJ3n"
        "moQK5oyH5LukLmxpc3QgdmFyIOazleacr++8muaY"
        "vuekuuaJgOacieS/neWtmOeahOmqsOWtkOS4re+8"
        "jOWQjeensOW4puacieKAnOazleacr+KAneeahArm"
        "jIfku6QubGlzdCBhbGzvvJrmmL7npLrmiYDmnInk"
        "v53lrZjnmoTpqrDlrZDlkozlj5jph48K5rOo5oSP"
        "77yaLmxpc3QgYWxs5Lit55qEYWxs5LiN6IO9566A"
        "5YaZ");

    list_call_t list_all = [](
        protocol_list const & self,
        std::string const & message,
        event_info const & event,
        std::string & response) -> bool{
        profile::sptr_user_profile upf = profile::profile_manager::get_instance()->get_profile(event.user_id);

        auto def_rolls = upf->get_map<profile::def_roll_type,std::string>();
        auto user_vars = upf->get_map<std::string,profile::var_pair>();
        auto macro_rolls = upf->get_map<std::string,std::string>();

        output_constructor oc(event.nickname);
        oc.append_message(u8"的个人信息如下:");
        self.defr_msg(*def_rolls,u8"默认骰子:",oc);
        self.macro_msg(*macro_rolls,u8"已设置下列骰子指令:","",oc);
        self.var_msg(*user_vars,u8"已设置下列变量:","",oc);
        response = oc.str();
        return true;
    };
    this->call_map.insert(call_map_value_t("all",list_all));

    list_call_t list_roll = [](
        protocol_list const & self,
        std::string const & message,
        event_info const & event,
        std::string & response) -> bool{
        profile::sptr_user_profile upf = profile::profile_manager::get_instance()->get_profile(event.user_id);

        auto def_rolls = upf->get_map<profile::def_roll_type,std::string>();
        auto macro_rolls = upf->get_map<std::string,std::string>();

        output_constructor oc(event.nickname);
        oc.append_message(u8"的个人信息如下:");
        self.defr_msg(*def_rolls,u8"默认骰子:",oc);
        self.macro_msg(*macro_rolls,u8"已设置下列骰子指令:",message,oc);
        response = oc.str();
        return true;
    };
    this->call_map.insert(call_map_value_t("roll",list_roll));
    this->call_map.insert(call_map_value_t("r",list_roll));

    list_call_t list_var = [](
        protocol_list const & self,
        std::string const & message,
        event_info const & event,
        std::string & response) -> bool{
        profile::sptr_user_profile upf = profile::profile_manager::get_instance()->get_profile(event.user_id);

        auto user_vars = upf->get_map<std::string,profile::var_pair>();

        output_constructor oc(event.nickname);
        oc.append_message(u8"的个人信息如下:");
        self.var_msg(*user_vars,u8"已设置下列变量:",message,oc);
        response = oc.str();
        return true;
    };
    this->call_map.insert(call_map_value_t("var",list_var));
    this->call_map.insert(call_map_value_t("v",list_var));
}

bool protocol_list::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){
    
    std::smatch m;
    std::regex_search(message,m,this->filter_command);
    if(m.size() > 0){
        std::string message_cp = m[1];
        std::string args = m.suffix();

        std::transform(message_cp.begin(),message_cp.end(),message_cp.begin(),tolower);

        auto iter = this->call_map.find(message_cp);
        if(iter != this->call_map.end()) return iter->second(*this,args,event,response);
    }
    return false;
}
#pragma endregion

#pragma region delete
protocol_delete::protocol_delete(){
    this->is_stand_alone = false;
    this->filter_command = std::regex("^(all|v(?:ar)?|r(?:oll)?) *");
    this->identifier_regex = "d(?:elete)?";
    this->identifier_list ={"d","delete"};
    
    this->help_message = base64::decode(
        "5Yig6Zmk5bey5L+d5a2Y55qE6aqw5a2Q5ZKM5Y+Y"
        "6YePKC5kZWxldGXmiJbogIUuZCkK5oyH5LukLmRl"
        "bGV0ZSByb2xs77ya5Yig6Zmk5omA5pyJ5L+d5a2Y"
        "55qE6aqw5a2QCuaMh+S7pC5kcu+8muS4iui/sOaM"
        "h+S7pOeahOeugOWGmeW9ouW8jwrmjIfku6QuZGVs"
        "ZXRlIHZhcu+8muWIoOmZpOaJgOacieS/neWtmOea"
        "hOWPmOmHjwrmjIfku6QuZHbvvJrkuIrov7DmjIfk"
        "u6TnmoTnroDlhpnlvaLlvI8K5oyH5LukLmRlbGV0"
        "ZSByb2xsIOWKm+mHj++8muWIoOmZpOWQjeensOS4"
        "uuKAnOWKm+mHj+KAneeahOmqsOWtkArmjIfku6Qu"
        "ZGVsZXRlIHZhciDms5XmnK/vvJrliKDpmaTlkI3n"
        "p7DkuLrigJzms5XmnK/igJ3nmoTlj5jph48K5oyH"
        "5LukLmRlbGV0ZSBhbGzvvJrmmL7npLrmiYDmnInk"
        "v53lrZjnmoTpqrDlrZDlkozlj5jph48K5rOo5oSP"
        "77yaLmRlbGV0ZSBhbGzkuK3nmoRhbGzkuI3og73n"
        "roDlhpkK5rOo5oSP77ya6buY6K6k6aqw5a2Q5piv"
        "5peg5rOV5Yig6Zmk55qE");

    delete_call_t delete_all = [](
        protocol_delete const & self,
        std::string const & message,
        event_info const & event,
        std::string & response) -> bool{
            profile::profile_manager * pfm = profile::profile_manager::get_instance();
            profile::sptr_user_profile upf = pfm->get_profile(event.user_id);

            auto user_vars = upf->get_map<std::string,profile::var_pair>();
            auto macro_rolls = upf->get_map<std::string,std::string>();

            user_vars->clear();
            macro_rolls->clear();
            pfm->force_update(event.user_id);

            output_constructor oc(event.nickname);
            oc.append_message(u8"已删除所有骰子指令和变量。");
            response = oc.str();
            return true;
        };
    this->call_map.insert(call_map_value_t("all",delete_all));

    delete_call_t delete_var = [](
        protocol_delete const & self,
        std::string const & message,
        event_info const & event,
        std::string & response) -> bool{
            profile::profile_manager * pfm = profile::profile_manager::get_instance();
            profile::sptr_user_profile upf = pfm->get_profile(event.user_id);

            auto user_vars = upf->get_map<std::string,profile::var_pair>();
            if(message.size()>0){
                auto iter = user_vars->find(message);
                if(iter == user_vars->end()) return false;
                user_vars->erase(iter);
                pfm->force_update(event.user_id);

                output_constructor oc(event.nickname);
                oc.append_message(u8"已删除变量:");
                oc.append_message(message);
                response = oc.str();
                return true;
            }
            else{
                user_vars->clear();
                pfm->force_update(event.user_id);

                output_constructor oc(event.nickname);
                oc.append_message(u8"已删除所有变量。");
                response = oc.str();
                return true;
            }
        };
    this->call_map.insert(call_map_value_t("var",delete_var));
    this->call_map.insert(call_map_value_t("v",delete_var));

    delete_call_t delete_roll = [](
        protocol_delete const & self,
        std::string const & message,
        event_info const & event,
        std::string & response) -> bool{
            profile::profile_manager * pfm = profile::profile_manager::get_instance();
            profile::sptr_user_profile upf = pfm->get_profile(event.user_id);

            auto macro_rolls = upf->get_map<std::string,std::string>();
            if(message.size()>0){
                auto iter = macro_rolls->find(message);
                if(iter == macro_rolls->end()) return false;
                macro_rolls->erase(iter);
                pfm->force_update(event.user_id);
                
                output_constructor oc(event.nickname);
                oc.append_message(u8"已删除骰子指令:");
                oc.append_message(message);
                response = oc.str();
                return true;
            }
            else{
                macro_rolls->clear();
                pfm->force_update(event.user_id);

                output_constructor oc(event.nickname);
                oc.append_message(u8"已删除所有骰子指令。");
                response = oc.str();
                return true;
            }
        };
    this->call_map.insert(call_map_value_t("roll",delete_roll));
    this->call_map.insert(call_map_value_t("r",delete_roll));
}

bool protocol_delete::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){

    std::smatch m;
    std::regex_search(message,m,this->filter_command);
    if(m.size() > 0){
        std::string message_cp = m[1];
        std::string args = m.suffix();

        std::transform(message_cp.begin(),message_cp.end(),message_cp.begin(),tolower);

        auto iter = this->call_map.find(message_cp);
        if(iter != this->call_map.end()) return iter->second(*this,args,event,response);
    }
    return false;
}
#pragma endregion
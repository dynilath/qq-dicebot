#pragma once

#include "./common.h"
#include "./dice_utils.h"
#include "./number.h"

#include <memory>

#include "./dice_roller.h"
#include "./protocol_manager.h"
#include "./protocol_base.h"
#include "./protocol_nickname.h"
#include "./protocol_roll_dice.h"
#include "./protocol_profile.h"
#include "./protocol_manual_dice.h"
#include "./protocol_specialized_dice.h"
#include "./nick_manager.h"
#include "./database_manager.h"
#include "./manual_dice_control.h"
#include "./profile_manager.h"

namespace dicebot{
    protocol_manager * dice_protocols;
    nickname::nickname_manager * nick_ctrl;
    database::database_manager * db_ctrl;
    manual::manual_dice_control * md_ctrl;
    profile::profile_manager * pf_ctrl;
    
    void initialize(std::string app_dir){
        is_no_sql_mode = false;
        db_ctrl = new database::database_manager(app_dir);
        nick_ctrl = new nickname::nickname_manager();
        md_ctrl = new manual::manual_dice_control();
        pf_ctrl = new profile::profile_manager();

        roll::random_initialize();
        dice_protocols = new protocol_manager();
        dice_protocols->register_dice(std::make_shared<protocol::protocol_roll_dice>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_coc_dice>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_manual_dice>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_nickname>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_wod_dice>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_fate_dice>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_set_roll>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_set_var>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_list>());
        dice_protocols->register_dice(std::make_shared<protocol::protocol_delete>());
        dice_protocols->finish_initialization();
    }

    void salvage(){
        if(dice_protocols) delete dice_protocols;
        if(nick_ctrl) delete nick_ctrl;
        if(db_ctrl) delete db_ctrl;
        if(md_ctrl) delete md_ctrl;
    }

    void set_logger(std::function<void(std::string,std::string)> varlog){
        logger::_log = varlog;
    }

    bool try_fill_nickname(event_info & event){
        return nickname::nickname_manager::instance->get_nickname(event);
    }

    bool message_pipeline(
        std::string const & source,
        event_info & event,
        std::string & output){

        std::list<std::string> source_splits;

        utils::split_line(source, source_splits);

        std::list<std::string>::iterator iter_source = source_splits.begin();

        ostrs ot(ostrs::ate);

        bool is_output_available = false;
        bool does_last_line_have_output = false;

        for (; iter_source != source_splits.end(); iter_source++) {
            
            size_t pos_of_content;

            if(!utils::find_point(*iter_source,pos_of_content)) continue;

            std::string content = (*iter_source).substr(pos_of_content);

            std::smatch match_command;

            std::regex_search(
                content,
                match_command, 
                *(dice_protocols->get_regex_command()));

            if(match_command.begin() == match_command.end()) continue;

            std::string command = match_command[1];
            protocol::p_protocol target_protocol = dice_protocols->get_protocol(command);

            if(target_protocol->is_stand_alone){
                if(target_protocol->resolve_request(match_command.suffix().str(),event,output)){
                    return is_output_available = true;
                }
            }
            else{
                std::string response;
                if(target_protocol->resolve_request(match_command.suffix().str(),event,response)){
                    if(is_output_available) ot << std::endl;
                    ot << response;
                    is_output_available = true;
                    continue;
                }
            }
        }

        if(is_output_available) {
            output.assign(ot.str());
        }
        return is_output_available;
    }
}
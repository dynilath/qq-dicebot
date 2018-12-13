#pragma once

#include "./common.h"
#include "./dice_utils.h"
#include "./number.h"

#include <memory>

#include "./dice_roller.h"
#include "./protocol_manager.h"
#include "./protocol_base.h"
#include "./protocol_coc_dice.h"
#include "./protocol_nickname.h"
#include "./protocol_roll_dice.h"
#include "./protocol_manual_dice.h"
#include "./nick_manager.h"
#include "./database_manager.h"
#include "./manual_dice_control.h"

namespace dicebot{
    protocol_manager * dice_protocols;
    nickname_manager * nickCtrl;
    database::database_manager * dbCtrl;
    manual_dice_control * mdCtrl;

	std::string str_help_message(u8"DiceBot by dynilath v1.4.2\r\n访问项目主页http://github.com/dynilath/CoolQDicebot\r\n获得使用说明以及了解更多内容。");
	
    void initialize(std::string app_dir){
        dbCtrl = new database::database_manager(app_dir);
        nickCtrl = new nickname_manager();
        mdCtrl = new manual_dice_control();

        roll::dice_roller::random_initialize();
        dice_protocols = new protocol_manager();
        dice_protocols->register_dice(std::shared_ptr<protocol::protocol_base>(new protocol::protocol_roll_dice()));
        dice_protocols->register_dice(std::shared_ptr<protocol::protocol_base>(new protocol::protocol_coc_dice()));
        dice_protocols->register_dice(std::shared_ptr<protocol::protocol_base>(new protocol::protocol_manual_dice()));
        dice_protocols->register_dice(std::shared_ptr<protocol::protocol_base>(new protocol::protocol_nickname()));
	    dice_protocols->create_command_regex();
    }

    void set_logger(std::function<void(std::string,std::string)> varlog){
        logger::log = varlog;
    }

    bool group_message_pipeline(
		std::string source,
		const int64_t fromGroup, 
		const int64_t fromQQ, 
		bool isfromGroup,
		std::string &output){

		std::vector<std::string> source_splits;

		if (source == ".help") {
			output.assign(str_help_message);
			return true;
		}

		split_line(source, source_splits);

		std::vector<std::string>::iterator iterator_sources = source_splits.begin();

		std::ostringstream ostrs_output_stream(std::ostringstream::ate);

		bool is_output_available = false;
		bool does_last_line_have_output = false;

		for (; iterator_sources != source_splits.end(); iterator_sources++) {
			std::smatch matchList_command_identifier_match;
			std::regex_search((*iterator_sources), matchList_command_identifier_match, *(dice_protocols->get_regex_command()));
			if (matchList_command_identifier_match.begin() == matchList_command_identifier_match.end()) continue;

			std::string command = matchList_command_identifier_match[1];
			protocol::protocol_base * target_protocol = dice_protocols->get_protocol(command);

			std::string response = target_protocol->resolve_request(
				matchList_command_identifier_match.suffix().str(),
				fromGroup,
				fromQQ,
				isfromGroup
			);

			if (response.size() > 0) {
				if (is_output_available) ostrs_output_stream << std::endl;
				ostrs_output_stream << response;
				is_output_available = true;
				continue;
			}

		}

		if (is_output_available) {
			output.assign(ostrs_output_stream.str());
		}
		return is_output_available;
	}
}
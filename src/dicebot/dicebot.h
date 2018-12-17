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

#include "../cqsdk/utils/base64.h"

namespace base64 = cq::utils::base64;

namespace dicebot{
    protocol_manager * dice_protocols;
    nickname_manager * nickCtrl;
    database::database_manager * dbCtrl;
    manual_dice_control * mdCtrl;

	std::string str_help_message("RGljZUJvdCUyMGJ5JTIwZHluaWxhdGglMjB2MS40LjIlNUNyJTVDbiV1OEJCRiV1OTVFRSV1OTg3OSV1NzZFRSV1NEUzQiV1OTg3NWh0dHAlM0EvL2dpdGh1Yi5jb20vZHluaWxhdGgvQ29vbFFEaWNlYm90JTVDciU1Q24ldTgzQjcldTVGOTcldTRGN0YldTc1MjgldThCRjQldTY2MEUldTRFRTUldTUzQ0EldTRFODYldTg5RTMldTY2RjQldTU5MUEldTUxODUldTVCQjkldTMwMDI=");
	
    void initialize(std::string app_dir){
        dbCtrl = new database::database_manager(app_dir);
        nickCtrl = new nickname_manager();
        mdCtrl = new manual_dice_control();

        roll::random_initialize();
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

	bool try_get_nickname(int64_t group_id, int64_t user_id, std::string &str_out_nick){
		str_out_nick.assign("");
		return true;
	}

    bool group_message_pipeline(
		std::string source,
		const int64_t fromGroup, 
		const int64_t fromQQ, 
		bool isfromGroup,
		std::string &output){

		std::vector<std::string> source_splits;

		if (source == ".help") {
			output.assign(base64::decode(str_help_message));
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